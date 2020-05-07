#include "pch.hpp"
#include "xmlMapParser.hpp"
#include "Components/physicsComponents.hpp"
#include "Components/debugComponents.hpp"
#include "Renderer/renderer.hpp"
#include "AI/aiManager.hpp"
#include "Utilities/csv.hpp"
#include "Utilities/filePath.hpp"

namespace ph {

void XmlMapParser::parseFile(const Xml& mapNode, AIManager& aiManager, entt::registry& gameRegistry, EntitiesTemplateStorage& templates)
{
	mGameRegistry = &gameRegistry;
	mTemplates = &templates;

	//load denial areas
	auto objectGroups = mapNode.getChildren("objectgroup");
	for(auto& objectGroup : objectGroups)
	{
		if(objectGroup.getAttribute("name")->toString() == "denialAreas")
		{
			auto objects = objectGroup.getChildren("object");
			for(auto& object : objects)
			{
				auto getBounds = [&object]
				{
					return FloatRect(object.getAttribute("x")->toFloat(), object.getAttribute("y")->toFloat(),
					                 object.getAttribute("width")->toFloat(), object.getAttribute("height")->toFloat());
				};

				if(object.getAttribute("type")->toString() == "CollisionAndLightWallDenialArea")
					mDenialAreas.collisionsAndLightWalls.emplace_back(getBounds());
				else if(object.getAttribute("type")->toString() == "CollisionDenialArea")
					mDenialAreas.collisions.emplace_back(getBounds());
				else if(object.getAttribute("type")->toString() == "LightWallDenialArea")
					mDenialAreas.lightWalls.emplace_back(getBounds());
			}
		}
	}

	#ifndef PH_DISTRIBUTION
	{
		// load denial areas to registry for debug visualization purposes

		using component::DenialArea;

		auto createDenialArea = [this](FloatRect area, DenialArea::Type type)
		{
			auto entity = mGameRegistry->create();
			mGameRegistry->assign<component::BodyRect>(entity, area);
			mGameRegistry->assign<component::DenialArea>(entity, DenialArea{type});
		};

		for(auto& area : mDenialAreas.collisions)
			createDenialArea(area, DenialArea::Collision);

		for(auto& area : mDenialAreas.lightWalls)
			createDenialArea(area, DenialArea::LightWall);

		for(auto& area : mDenialAreas.collisionsAndLightWalls)
			createDenialArea(area, DenialArea::All);
		#endif
	}

	GeneralMapInfo info = getGeneralMapInfo(mapNode);

	aiManager.registerMapSize(static_cast<sf::Vector2u>(info.mapSize));
	aiManager.registerTileSize(info.tileSize);

	mRenderChunks.reserve(static_cast<size_t>(info.nrOfChunks));
	mChunkCollisions.reserve(static_cast<size_t>(info.nrOfChunks));

	const std::vector<Xml> tilesetNodes = mapNode.getChildren("tileset");
	PH_ASSERT_WARNING(tilesetNodes.size() != 0, "Map doesn't have any tilesets");

	const TilesetsData tilesetsData = getTilesetsData(tilesetNodes);
	const std::vector<Xml> layerNodes = getLayerNodes(mapNode);
	
	// parse map layers
	FloatRect mapBounds;
	unsigned char z = sLowestLayerZ;
	bool isFirstChunk = true;
	for (const Xml& layerNode : layerNodes)
	{
		Xml dataNode = *layerNode.getChild("data");
		std::string encoding = dataNode.getAttribute("encoding")->toString();
		PH_ASSERT_CRITICAL(encoding == "csv", "Used unsupported data encoding: " + encoding);
		auto layerName = layerNode.getAttribute("name")->toString();
		bool outdoor = layerName.find("indoor") == std::string::npos;
		for(Xml& chunkNode : dataNode.getChildren("chunk"))
		{
			sf::Vector2f chunkPos(chunkNode.getAttribute("x")->toFloat(), chunkNode.getAttribute("y")->toFloat());
			sf::Vector2f chunkSize(chunkNode.getAttribute("width")->toFloat(), chunkNode.getAttribute("height")->toFloat());

			PH_ASSERT_CRITICAL(chunkSize.x == 12.f, "You have to set map parameter \"Output Chunk Width\" to 12!");
			PH_ASSERT_CRITICAL(chunkSize.y == 12.f, "You have to set map parameter \"Output Chunk Height\" to 12!");

			if(isFirstChunk) 
			{
				mapBounds.pos = chunkPos;
				mapBounds.size = chunkSize;
				isFirstChunk = false;
			}
			else
			{
				if(chunkPos.x < mapBounds.x) {
					mapBounds.x = chunkPos.x;
				}
				if(chunkPos.y < mapBounds.y) {
					mapBounds.y = chunkPos.y;
				}
				
				float mapWidthToThisChunk = chunkPos.x - mapBounds.x + chunkSize.x;
				if(mapWidthToThisChunk > mapBounds.w) {
					mapBounds.w = mapWidthToThisChunk; 
				}
				float mapHeightToThisChunk = chunkPos.y - mapBounds.y + chunkSize.y;
				if(mapHeightToThisChunk > mapBounds.h) {
					mapBounds.h = mapHeightToThisChunk;
				}
			}

			auto globalIds = Csv::toUnsigneds(chunkNode.toString());
			createChunk(chunkPos, globalIds, tilesetsData, info, z, aiManager, outdoor);
		}
		--z;
	}

	// translate map bounds to world space
	mapBounds.x *= info.tileSize.x;
	mapBounds.y *= info.tileSize.y;
	mapBounds.w *= info.tileSize.x;
	mapBounds.h *= info.tileSize.y;
	
	auto createBorderCollision = [this]() -> FloatRect& {
		auto borderEntity = mTemplates->createCopy("BorderCollision", *mGameRegistry);
		createDebugName(borderEntity, "map border");
		return mGameRegistry->get<component::BodyRect>(borderEntity);
	};

	// create left map border
	auto& leftBorderRect = createBorderCollision();
	leftBorderRect = FloatRect(mapBounds.x - info.tileSize.x, mapBounds.y - info.tileSize.y, info.tileSize.x, mapBounds.h + 2 * info.tileSize.y);

	// create top map border
	auto& topBorderRect = createBorderCollision();
	topBorderRect = FloatRect(mapBounds.x - info.tileSize.x, mapBounds.y - info.tileSize.y, mapBounds.w + 2 * info.tileSize.x, info.tileSize.y);

	// create right map border
	auto& rightBorderRect = createBorderCollision();
	rightBorderRect = FloatRect(mapBounds.w + mapBounds.x, -info.tileSize.y + mapBounds.y, info.tileSize.x, mapBounds.h + 2 * info.tileSize.y);

	// create bottom map border
	auto& bottomBorderRect = createBorderCollision();
	bottomBorderRect = FloatRect(mapBounds.x - info.tileSize.x, mapBounds.h + mapBounds.y, mapBounds.w + 2 * info.tileSize.x, info.tileSize.y);
}

auto XmlMapParser::getGeneralMapInfo(const Xml& mapNode) const -> GeneralMapInfo
{
	const std::string orientation = mapNode.getAttribute("orientation")->toString();
	PH_ASSERT_CRITICAL(orientation == "orthogonal", "Used unsupported map orientation: " + orientation);
	PH_ASSERT_CRITICAL(mapNode.getAttribute("infinite")->toBool(), "Now we support only infinite maps!");

	GeneralMapInfo info;
	info.mapSize.x = mapNode.getAttribute("width")->toFloat();
	info.mapSize.y = mapNode.getAttribute("height")->toFloat();
	info.tileSize.x = mapNode.getAttribute("tilewidth")->toFloat();
	info.tileSize.y = mapNode.getAttribute("tileheight")->toFloat();
	info.nrOfChunksInOneRow = std::ceil(info.mapSize.x / sChunkSize);
	info.nrOfChunksInOneColumn = std::ceil(info.mapSize.y / sChunkSize);
	info.nrOfChunks = info.nrOfChunksInOneRow * info.nrOfChunksInOneColumn;
	return info;
}

auto XmlMapParser::getTilesetsData(const std::vector<Xml>& tilesetNodes) const -> const TilesetsData
{
	TilesetsData tilesets;
	tilesets.firstGlobalTileIds.reserve(tilesetNodes.size());
	tilesets.tileCounts.reserve(tilesetNodes.size());
	tilesets.columnsCounts.reserve(tilesetNodes.size());

	for(Xml tilesetNode : tilesetNodes) 
	{
		const unsigned firstGlobalTileId = tilesetNode.getAttribute("firstgid")->toUnsigned();
		tilesets.firstGlobalTileIds.emplace_back(firstGlobalTileId);
		if(auto source = tilesetNode.getAttribute("source")) {
			std::string tilesetNodeSource = source->toString();
			tilesetNodeSource = FilePath::toFilename(tilesetNodeSource, '/');
			PH_LOG_INFO("Detected not embedded tileset in Map: " + tilesetNodeSource);
			Xml tilesetDocument;
			PH_ASSERT_CRITICAL(tilesetDocument.loadFromFile("scenes/map/" + tilesetNodeSource),
				"Not embedded tileset file \"" + tilesetNodeSource + "\" wasn't loaded correctly!");
			tilesetNode = *tilesetDocument.getChild("tileset");
		}
		tilesets.tileCounts.emplace_back(tilesetNode.getAttribute("tilecount")->toUnsigned());
		tilesets.columnsCounts.emplace_back(tilesetNode.getAttribute("columns")->toUnsigned());
		const Xml imageNode = *tilesetNode.getChild("image");
		tilesets.tilesetFileName = FilePath::toFilename(imageNode.getAttribute("source")->toString(), '/');
		const std::vector<Xml> tileNodes = tilesetNode.getChildren("tile");
		TilesData tilesData = getTilesData(tileNodes);
		tilesData.firstGlobalTileId = firstGlobalTileId;
		tilesets.tilesData.emplace_back(tilesData);
	}

	return tilesets;
}

auto XmlMapParser::getTilesData(const std::vector<Xml>& tileNodes) const -> TilesData
{
	TilesData tilesData{};
	tilesData.ids.reserve(tileNodes.size());
	tilesData.bounds.reserve(tileNodes.size());
	for(const Xml& tileNode : tileNodes) 
	{
		if(auto objectGroupNode = tileNode.getChild("objectgroup"))
		{
			tilesData.ids.emplace_back(tileNode.getAttribute("id")->toUnsigned());
			const auto objectNodes = objectGroupNode->getChildren("object");
			std::vector<FloatRect> collisions;
			std::vector<FloatRect> lightWalls;
			for(auto& objectNode : objectNodes)
			{
				if(auto type = objectNode.getAttribute("type"))
				{
					auto getBounds = [&objectNode] 
					{
						auto width = objectNode.getAttribute("width");
						auto height = objectNode.getAttribute("height");
						return FloatRect(
							objectNode.getAttribute("x")->toFloat(),
							objectNode.getAttribute("y")->toFloat(),
							width ? width->toFloat() : 0.f,
							height ? height->toFloat() : 0.f
						);
					};

					std::string typeStr = type->toString();
					if(typeStr == "Collision")
						collisions.emplace_back(getBounds());
					else if(typeStr == "LightWall")
						lightWalls.emplace_back(getBounds());
				}
			}
			tilesData.bounds.emplace_back(collisions);
			tilesData.lightWalls.emplace_back(lightWalls);
		}
	}
	return tilesData;
}

std::vector<Xml> XmlMapParser::getLayerNodes(const Xml& mapNode) const
{
	const std::vector<Xml> layerNodes = mapNode.getChildren("layer");
	if(layerNodes.size() == 0)
		PH_LOG_WARNING("Map doesn't have any layers");
	return layerNodes;
}

void XmlMapParser::createChunk(sf::Vector2f chunkPos, const std::vector<unsigned>& globalTileIds, const TilesetsData& tilesets,
                               const GeneralMapInfo& info, unsigned char z, AIManager& aiManager, bool outdoor)
{
	PH_PROFILE_FUNCTION();

	std::vector<ChunkQuadData> quads;
	std::vector<FloatRect> lightWalls;
	std::vector<FloatRect> chunkCollisionRects;
	FloatRect quadsBounds = FloatRect(chunkPos.x, chunkPos.y, sChunkSize, sChunkSize);
	FloatRect lightWallsBounds = {};

	for (size_t tileIndexInChunk = 0; tileIndexInChunk < globalTileIds.size(); ++tileIndexInChunk) 
	{
		constexpr unsigned bitsInByte = 8;
		const unsigned flippedHorizontally = 1u << (sizeof(unsigned) * bitsInByte - 1);
		const unsigned flippedVertically = 1u << (sizeof(unsigned) * bitsInByte - 2);
		const unsigned flippedDiagonally = 1u << (sizeof(unsigned) * bitsInByte - 3);

		const bool isHorizontallyFlipped = globalTileIds[tileIndexInChunk] & flippedHorizontally;
		const bool isVerticallyFlipped = globalTileIds[tileIndexInChunk] & flippedVertically;
		const bool isDiagonallyFlipped = globalTileIds[tileIndexInChunk] & flippedDiagonally;

		const unsigned globalTileId = globalTileIds[tileIndexInChunk] & (~(flippedHorizontally | flippedVertically | flippedDiagonally));

		bool hasTile = globalTileId != 0;
		if (hasTile) 
		{
			size_t tilesetIndex = findTilesetIndex(globalTileId, tilesets);
			if (tilesetIndex == std::string::npos) {
				PH_LOG_WARNING("It was not possible to find tileset for " + std::to_string(globalTileId));
				continue;
			}

			sf::Vector2f positionInTiles(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex((unsigned)tileIndexInChunk, (unsigned)sChunkSize));
			positionInTiles += static_cast<sf::Vector2f>(chunkPos);

			// create quad data
			ChunkQuadData cqd;

			sf::Vector2f tileWorldPos( 
				positionInTiles.x * static_cast<float>(info.tileSize.x),
				positionInTiles.y * static_cast<float>(info.tileSize.y)
			);

			cqd.position = tileWorldPos; 

			// TODO: Replace rotate/size-textureRect stuff with texture coords
			auto tileSize = static_cast<sf::Vector2f>(info.tileSize);
			if(!(isHorizontallyFlipped || isVerticallyFlipped || isDiagonallyFlipped)) {
				cqd.size = tileSize;
				cqd.rotation = 0.f;
			}
			else if(isHorizontallyFlipped && isVerticallyFlipped && isDiagonallyFlipped) {
				cqd.size = {tileSize.x, -tileSize.y};
				cqd.position.x += tileSize.x;
				cqd.rotation = 270.f;
			}
			else if(isHorizontallyFlipped && isVerticallyFlipped) {
				cqd.size = -tileSize;
				cqd.position += tileSize;
				cqd.rotation = 0.f;
			}
			else if(isHorizontallyFlipped && isDiagonallyFlipped) {
				cqd.size = tileSize;
				cqd.rotation = 90.f;
			}
			else if(isVerticallyFlipped && isDiagonallyFlipped) {
				cqd.size = tileSize;
				cqd.rotation = 270.f;
			}
			else if(isHorizontallyFlipped) {
				cqd.size = {-tileSize.x, tileSize.y};
				cqd.position.x += tileSize.x;
				cqd.rotation = 0.f;
			}
			else if(isVerticallyFlipped) {
				cqd.size = {tileSize.x, -tileSize.y};
				cqd.position.y += tileSize.y;
				cqd.rotation = 0.f;
			}
			else if(isDiagonallyFlipped) {
				cqd.size = {-tileSize.x, tileSize.y};
				cqd.position.y -= tileSize.x;
				cqd.rotation = 270.f;
			}
			cqd.rotation = Math::degreesToRadians(cqd.rotation);

			const unsigned tileId = globalTileId - tilesets.firstGlobalTileIds[tilesetIndex];
			auto tileRectPosition = static_cast<sf::Vector2f>(
				Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(tileId, tilesets.columnsCounts[tilesetIndex]));
			tileRectPosition.x *= (info.tileSize.x + 2);
			tileRectPosition.y *= (info.tileSize.y + 2);
			tileRectPosition.x += 1;
			tileRectPosition.y += 1;
			const sf::Vector2f textureSize(576.f, 576.f); // TODO: Make it not hardcoded like that
			cqd.textureRect.x = tileRectPosition.x / textureSize.x;
			cqd.textureRect.y = (textureSize.y - tileRectPosition.y - info.tileSize.y) / textureSize.y;
			cqd.textureRect.w = static_cast<float>(info.tileSize.x) / textureSize.x;
			cqd.textureRect.h = static_cast<float>(info.tileSize.y) / textureSize.y;

			// emplace quad data to chunk
			quads.emplace_back(cqd);

			// load collision bodies and light walls
			size_t tilesDataIndex = findTilesIndex(tilesets.firstGlobalTileIds[tilesetIndex], tilesets.tilesData);
			if (tilesDataIndex == std::string::npos)
				continue;
			auto& tilesData = tilesets.tilesData[tilesDataIndex];
			for (std::size_t i = 0; i < tilesData.ids.size(); ++i) 
			{
				if (tileId == tilesData.ids[i]) 
				{
					// collision bodies
					for(FloatRect collisionRect : tilesData.bounds[i])
					{
						if(isHorizontallyFlipped)
							collisionRect.x = info.tileSize.x - collisionRect.x - collisionRect.w;
						if(isVerticallyFlipped)
							collisionRect.y = info.tileSize.y - collisionRect.y - collisionRect.h;

						collisionRect.x += tileWorldPos.x; 
						collisionRect.y += tileWorldPos.y; 

						bool shouldBeAdded = true;

						for(FloatRect collisionDenialArea : mDenialAreas.collisionsAndLightWalls)
							if(intersect(collisionDenialArea, collisionRect)) {
								shouldBeAdded = false;
								break;
							}

						if(shouldBeAdded)
							for(FloatRect collisionDenialArea : mDenialAreas.collisions)
								if(intersect(collisionDenialArea, collisionRect)) {
									shouldBeAdded = false;
									break;
								}

						if(shouldBeAdded)
							chunkCollisionRects.emplace_back(collisionRect);
					}

					// light walls
					for(FloatRect lightWallRect : tilesData.lightWalls[i])
					{
						if(isHorizontallyFlipped)
							lightWallRect.x = info.tileSize.x - lightWallRect.x - lightWallRect.w;
						if(isVerticallyFlipped)
							lightWallRect.y = info.tileSize.y - lightWallRect.y - lightWallRect.h;

						lightWallRect.x += tileWorldPos.x; 
						lightWallRect.y += tileWorldPos.y; 

						bool shouldBeAdded = true;
						for(FloatRect lightWallDenialArea : mDenialAreas.collisionsAndLightWalls)
							if(intersect(lightWallDenialArea, lightWallRect)) {
								shouldBeAdded = false;
								break;
							}

						if(shouldBeAdded)
							for(FloatRect lightWallDenialArea : mDenialAreas.lightWalls)
								if(intersect(lightWallDenialArea, lightWallRect))
									shouldBeAdded = false;

						if(shouldBeAdded)
							lightWalls.emplace_back(lightWallRect);
					}

					break;
				}
			}		
		}
	}

	if(!quads.empty())
	{
		// transform chunk bounds to world coords so we can later use them for culling in RenderSystem
		quadsBounds.x *= static_cast<float>(info.tileSize.x);
		quadsBounds.y *= static_cast<float>(info.tileSize.y);
		quadsBounds.w *= static_cast<float>(info.tileSize.x);
		quadsBounds.h *= static_cast<float>(info.tileSize.y);

		// set light walls bounds so we can do culling in RenderSystem
		lightWallsBounds.x = quadsBounds.x - 400.f;
		lightWallsBounds.y = quadsBounds.y - 400.f;
		lightWallsBounds.w = quadsBounds.w + 800.f;
		lightWallsBounds.h = quadsBounds.h + 800.f;

		// check should we construct ground chunk or normal chunk 
		FloatRect groundTextureRect = quads[0].textureRect;
		bool groundChunkShouldBeConstructed = false;
		if(quads.size() == 144)
		{
			if(lightWalls.empty())
			{
				groundChunkShouldBeConstructed = true;
				for(auto& quad : quads)
				{
					if(quad.textureRect != groundTextureRect)
					{
						groundChunkShouldBeConstructed = false;
						break;
					}
				}
			}
		}

		// construct chunk in the registry
		if(groundChunkShouldBeConstructed)
		{
			auto groundChunkEntity = mTemplates->createCopy("GroundMapChunk", *mGameRegistry);
			createDebugName(groundChunkEntity, "ground chunk");

			auto& grc = mGameRegistry->get<component::GroundRenderChunk>(groundChunkEntity);
			grc.bounds = quadsBounds;
			grc.textureRect = groundTextureRect;
			grc.z = z;

			if(outdoor)
			{
				auto& ob = mGameRegistry->assign<component::OutdoorBlend>(groundChunkEntity);
				ob.darkness = 1.f;
			}
			else
			{
				auto& ib = mGameRegistry->assign<component::IndoorBlend>(groundChunkEntity);
				ib.alpha = 0.f;
			}
		}
		else
		{
			auto chunkEntity = mTemplates->createCopy("MapChunk", *mGameRegistry);
			createDebugName(chunkEntity, "chunk");

			auto& rc = mGameRegistry->get<component::RenderChunk>(chunkEntity);
			rc.quads = quads;
			rc.lightWalls = lightWalls;
			rc.quadsBounds = quadsBounds;
			rc.lightWallsBounds = lightWallsBounds;
			rc.z = z;
			rc.rendererID = Renderer::registerNewChunk(quadsBounds);

			if(outdoor)
			{
				auto& ob = mGameRegistry->assign<component::OutdoorBlend>(chunkEntity);
				ob.darkness = 1.f;
			}
			else
			{
				auto& ib = mGameRegistry->assign<component::IndoorBlend>(chunkEntity);
				ib.alpha = 0.f;
			}

			auto& mscb = mGameRegistry->get<component::MultiStaticCollisionBody>(chunkEntity);
			mscb.rects = chunkCollisionRects;
			mscb.sharedBounds = quadsBounds;
		}
	}
}

std::size_t XmlMapParser::findTilesetIndex(const unsigned globalTileId, const TilesetsData& tilesets) const
{
	for (size_t i = 0; i < tilesets.firstGlobalTileIds.size(); ++i) {
		const unsigned firstGlobalTileId = tilesets.firstGlobalTileIds[i];
		const unsigned lastGlobalTileId = firstGlobalTileId + tilesets.tileCounts[i] - 1;
		if (globalTileId >= firstGlobalTileId && globalTileId <= lastGlobalTileId)
			return i;
	}
	return std::string::npos;
}

std::size_t XmlMapParser::findTilesIndex(const unsigned firstGlobalTileId, const std::vector<TilesData>& tilesData) const
{
	for (size_t i = 0; i < tilesData.size(); ++i)
		if (firstGlobalTileId == tilesData[i].firstGlobalTileId)
			return i;
	return std::string::npos;
}

// TODO: This function is repeated in TiledParser
void XmlMapParser::createDebugName(entt::entity entity, const char* name) const
{
	#ifndef PH_DISTRIBUTION
	
	auto& debugName = mGameRegistry->assign<component::DebugName>(entity);	
	memcpy(debugName.name, name, strlen(name));

	#endif
}



}

