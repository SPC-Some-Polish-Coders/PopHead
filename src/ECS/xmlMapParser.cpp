#include "xmlMapParser.hpp"
#include "Logs/logs.hpp"
#include "Utilities/profiling.hpp"
#include "Components/physicsComponents.hpp"
#include "Renderer/renderer.hpp"
#include "AI/aiManager.hpp"
#include "Utilities/xml.hpp"
#include "Utilities/csv.hpp"
#include "Utilities/filePath.hpp"
#include "Utilities/math.hpp"
#include <cmath>

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


	GeneralMapInfo info = getGeneralMapInfo(mapNode);

	aiManager.registerMapSize(static_cast<sf::Vector2u>(info.mapSize));
	aiManager.registerTileSize(info.tileSize);

	if(info.isMapInfinite)
	{
		mRenderChunks.reserve(static_cast<size_t>(info.nrOfChunks));
		mChunkCollisions.reserve(static_cast<size_t>(info.nrOfChunks));
	}
	else
	{
		mRenderChunks.resize(static_cast<size_t>(info.nrOfChunks));
		mChunkCollisions.resize(static_cast<size_t>(info.nrOfChunks));
	}

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
		if(info.isMapInfinite)
		{
			for(Xml& chunkNode : dataNode.getChildren("chunk"))
			{
				sf::Vector2f chunkPos(chunkNode.getAttribute("x")->toFloat(), chunkNode.getAttribute("y")->toFloat());
				sf::Vector2f chunkSize(chunkNode.getAttribute("width")->toFloat(), chunkNode.getAttribute("height")->toFloat());

				PH_ASSERT_CRITICAL(chunkSize.x == 12.f, "You have to set map parameter \"Output Chunk Width\" to 12!");
				PH_ASSERT_CRITICAL(chunkSize.y == 12.f, "You have to set map parameter \"Output Chunk Height\" to 12!");

				if(isFirstChunk) 
				{
					mapBounds.setPosition(chunkPos);
					mapBounds.setSize(chunkSize);
					isFirstChunk = false;
				}
				else
				{
					if(chunkPos.x < mapBounds.left) {
						mapBounds.left = chunkPos.x;
					}
					if(chunkPos.y < mapBounds.top) {
						mapBounds.top = chunkPos.y;
					}
					
					float mapWidthToThisChunk = chunkPos.x - mapBounds.left + chunkSize.x;
					if(mapWidthToThisChunk > mapBounds.width) {
						mapBounds.width = mapWidthToThisChunk; 
					}
					float mapHeightToThisChunk = chunkPos.y - mapBounds.top + chunkSize.y;
					if(mapHeightToThisChunk > mapBounds.height) {
						mapBounds.height = mapHeightToThisChunk;
					}
				}

				auto globalIds = Csv::toUnsigneds(chunkNode.toString());
				createInfiniteMapChunk(chunkPos, globalIds, tilesetsData, info, z, aiManager);
			}
		}
		else
		{
			auto globalIds = Csv::toUnsigneds(dataNode.toString());
			createFiniteMapLayer(globalIds, tilesetsData, info, z, aiManager);
		}
		--z;
	}

	if(!info.isMapInfinite)
		mapBounds = {0.f, 0.f, info.mapSize.x, info.mapSize.y};

	// translate map bounds to world space
	mapBounds.left *= info.tileSize.x;
	mapBounds.top *= info.tileSize.y;
	mapBounds.width *= info.tileSize.x;
	mapBounds.height *= info.tileSize.y;
	
	auto createBorderCollision = [this]() -> FloatRect& {
		auto borderEntity = mTemplates->createCopy("BorderCollision", *mGameRegistry);
		auto& body = mGameRegistry->get<component::BodyRect>(borderEntity);
		return body.rect;
	};

	// create left map border
	auto& leftBorderRect = createBorderCollision();
	leftBorderRect = FloatRect(mapBounds.left - info.tileSize.x, mapBounds.top - info.tileSize.y, info.tileSize.x, mapBounds.height + 2 * info.tileSize.y);

	// create top map border
	auto& topBorderRect = createBorderCollision();
	topBorderRect = FloatRect(mapBounds.left - info.tileSize.x, mapBounds.top - info.tileSize.y, mapBounds.width + 2 * info.tileSize.x, info.tileSize.y);

	// create right map border
	auto& rightBorderRect = createBorderCollision();
	rightBorderRect = FloatRect(mapBounds.width + mapBounds.left, -info.tileSize.y + mapBounds.top, info.tileSize.x, mapBounds.height + 2 * info.tileSize.y);

	// create bottom map border
	auto& bottomBorderRect = createBorderCollision();
	bottomBorderRect = FloatRect(mapBounds.left - info.tileSize.x, mapBounds.height + mapBounds.top, mapBounds.width + 2 * info.tileSize.x, info.tileSize.y);
}

auto XmlMapParser::getGeneralMapInfo(const Xml& mapNode) const -> GeneralMapInfo
{
	const std::string orientation = mapNode.getAttribute("orientation")->toString();
	PH_ASSERT_CRITICAL(orientation == "orthogonal", "Used unsupported map orientation: " + orientation);

	GeneralMapInfo info;
	info.mapSize.x = mapNode.getAttribute("width")->toFloat();
	info.mapSize.y = mapNode.getAttribute("height")->toFloat();
	info.tileSize.x = mapNode.getAttribute("tilewidth")->toFloat();
	info.tileSize.y = mapNode.getAttribute("tileheight")->toFloat();
	info.isMapInfinite = mapNode.getAttribute("infinite")->toBool();
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

void XmlMapParser::createInfiniteMapChunk(sf::Vector2f chunkPos, const std::vector<unsigned>& globalTileIds, const TilesetsData& tilesets,
                                          const GeneralMapInfo& info, unsigned char z, AIManager& aiManager)
{
	PH_PROFILE_FUNCTION();

	std::vector<ChunkQuadData> quads;
	std::vector<FloatRect> lightWalls;
	std::vector<FloatRect> chunkCollisionRects;
	FloatRect quadsBounds = sf::FloatRect(chunkPos.x, chunkPos.y, sChunkSize, sChunkSize);
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
			cqd.textureRect.left = tileRectPosition.x / textureSize.x;
			cqd.textureRect.top = (textureSize.y - tileRectPosition.y - info.tileSize.y) / textureSize.y;
			cqd.textureRect.width = static_cast<float>(info.tileSize.x) / textureSize.x;
			cqd.textureRect.height = static_cast<float>(info.tileSize.y) / textureSize.y;

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
							collisionRect.left = info.tileSize.x - collisionRect.left - collisionRect.width;
						if(isVerticallyFlipped)
							collisionRect.top = info.tileSize.y - collisionRect.top - collisionRect.height;

						collisionRect.left += tileWorldPos.x; 
						collisionRect.top += tileWorldPos.y; 

						bool shouldBeAdded = true;

						for(FloatRect collisionDenialArea : mDenialAreas.collisionsAndLightWalls)
							if(collisionDenialArea.doPositiveRectsIntersect(collisionRect)) {
								shouldBeAdded = false;
								break;
							}

						if(shouldBeAdded)
							for(FloatRect collisionDenialArea : mDenialAreas.collisions)
								if(collisionDenialArea.doPositiveRectsIntersect(collisionRect)) {
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
							lightWallRect.left = info.tileSize.x - lightWallRect.left - lightWallRect.width;
						if(isVerticallyFlipped)
							lightWallRect.top = info.tileSize.y - lightWallRect.top - lightWallRect.height;

						lightWallRect.left += tileWorldPos.x; 
						lightWallRect.top += tileWorldPos.y; 

						bool shouldBeAdded = true;
						for(FloatRect lightWallDenialArea : mDenialAreas.collisionsAndLightWalls)
							if(lightWallDenialArea.doPositiveRectsIntersect(lightWallRect)) {
								shouldBeAdded = false;
								break;
							}

						if(shouldBeAdded)
							for(FloatRect lightWallDenialArea : mDenialAreas.lightWalls)
								if(lightWallDenialArea.doPositiveRectsIntersect(lightWallRect)) {
									shouldBeAdded = false;
								}

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
		quadsBounds.left *= static_cast<float>(info.tileSize.x);
		quadsBounds.top *= static_cast<float>(info.tileSize.y);
		quadsBounds.width *= static_cast<float>(info.tileSize.x);
		quadsBounds.height *= static_cast<float>(info.tileSize.y);

		// set light walls bounds so we can do culling in RenderSystem
		lightWallsBounds.left = quadsBounds.left - 400.f;
		lightWallsBounds.top = quadsBounds.top - 400.f;
		lightWallsBounds.width = quadsBounds.width + 800.f;
		lightWallsBounds.height = quadsBounds.height + 800.f;

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
			auto& grc = mGameRegistry->get<component::GroundRenderChunk>(groundChunkEntity);
			grc.bounds = quadsBounds;
			grc.textureRect = groundTextureRect;
			grc.z = z;
		}
		else
		{
			auto chunkEntity = mTemplates->createCopy("MapChunk", *mGameRegistry);
			auto& rc = mGameRegistry->get<component::RenderChunk>(chunkEntity);
			rc.quads = quads;
			rc.lightWalls = lightWalls;
			rc.quadsBounds = quadsBounds;
			rc.lightWallsBounds = lightWallsBounds;
			rc.z = z;
			rc.rendererID = Renderer::registerNewChunk(quadsBounds);

			auto& mscb = mGameRegistry->get<component::MultiStaticCollisionBody>(chunkEntity);
			mscb.rects = chunkCollisionRects;
			mscb.sharedBounds = quadsBounds;
		}
	}
}

void XmlMapParser::createFiniteMapLayer(const std::vector<unsigned>& globalTileIds, const TilesetsData& tilesets,
                                        const GeneralMapInfo& info, unsigned char z, AIManager& aiManager)
{
	PH_PROFILE_FUNCTION();

	// clear chunks 
	bool isFirstLayer = z == sLowestLayerZ;
	if(!isFirstLayer)
	{
		for(size_t i = 0; i < mRenderChunks.size(); ++i) {
			mRenderChunks[i] = component::RenderChunk();
			mChunkCollisions[i] = component::MultiStaticCollisionBody();	
		}
	}

	// fill chunks with z and bounds
	for(size_t i = 0; i < mRenderChunks.size(); ++i)
	{
		mRenderChunks[i].z = z;

		float row = std::floor(i / info.nrOfChunksInOneRow);
		mRenderChunks[i].quadsBounds = sf::FloatRect((sChunkSize * i) - (row * sChunkSize * info.nrOfChunksInOneRow),
		                                             row * sChunkSize, sChunkSize, sChunkSize);
	}

	for (size_t tileIndexInMap = 0; tileIndexInMap < globalTileIds.size(); ++tileIndexInMap) 
	{
		constexpr unsigned bitsInByte = 8;
		const unsigned flippedHorizontally = 1u << (sizeof(unsigned) * bitsInByte - 1);
		const unsigned flippedVertically = 1u << (sizeof(unsigned) * bitsInByte - 2);
		const unsigned flippedDiagonally = 1u << (sizeof(unsigned) * bitsInByte - 3);

		const bool isHorizontallyFlipped = globalTileIds[tileIndexInMap] & flippedHorizontally;
		const bool isVerticallyFlipped = globalTileIds[tileIndexInMap] & flippedVertically;
		const bool isDiagonallyFlipped = globalTileIds[tileIndexInMap] & flippedDiagonally;

		const unsigned globalTileId = globalTileIds[tileIndexInMap] & (~(flippedHorizontally | flippedVertically | flippedDiagonally));

		bool hasTile = globalTileId != 0;
		if (hasTile) 
		{
			const size_t tilesetIndex = findTilesetIndex(globalTileId, tilesets);
			if (tilesetIndex == std::string::npos) {
				PH_LOG_WARNING("It was not possible to find tileset for " + std::to_string(globalTileId));
				continue;
			}

			sf::Vector2f positionInTiles(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex((unsigned)tileIndexInMap, (unsigned)info.mapSize.x));

			// create quad data
			ChunkQuadData cqd;

			sf::Vector2f tileWorldPos(
				positionInTiles.x * static_cast<float>(info.tileSize.x),
				positionInTiles.y * static_cast<float>(info.tileSize.y)
			);

			cqd.position = tileWorldPos;

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
			cqd.textureRect.left = tileRectPosition.x / textureSize.x;
			cqd.textureRect.top = (textureSize.y - tileRectPosition.y - info.tileSize.y) / textureSize.y;
			cqd.textureRect.width = static_cast<float>(info.tileSize.x) / textureSize.x;
			cqd.textureRect.height = static_cast<float>(info.tileSize.y) / textureSize.y;

			// TODO: Optimize that
			// find chunk index
			size_t chunkIndex = 0;
			for(size_t i = 0; i < mRenderChunks.size(); ++i)
				if(mRenderChunks[i].quadsBounds.containsIncludingBounds(positionInTiles))
					chunkIndex = i;

			// emplace quad data to chunk
			mRenderChunks[chunkIndex].quads.emplace_back(cqd);

			// load collision bodies
			const size_t tilesDataIndex = findTilesIndex(tilesets.firstGlobalTileIds[tilesetIndex], tilesets.tilesData);
			if (tilesDataIndex == std::string::npos)
				continue;

			auto& tilesData = tilesets.tilesData[tilesDataIndex];
			for (size_t i = 0; i < tilesData.ids.size(); ++i) 
			{
				if (tileId == tilesData.ids[i]) 
				{
					// TODO: Add support for denial areas

					// collision bodies
					for(FloatRect collisionRect : tilesData.bounds[i])
					{
						if(isHorizontallyFlipped)
							collisionRect.left = info.tileSize.x - collisionRect.left - collisionRect.width;
						if(isVerticallyFlipped)
							collisionRect.top = info.tileSize.y - collisionRect.top - collisionRect.height;

						collisionRect.left += tileWorldPos.x; 
						collisionRect.top += tileWorldPos.y; 
						mChunkCollisions[chunkIndex].rects.emplace_back(collisionRect);
						aiManager.registerObstacle(positionInTiles);
						
					}

					// light walls
					for(FloatRect lightWallRect : tilesData.lightWalls[i])
					{
						if(isHorizontallyFlipped)
							lightWallRect.left = info.tileSize.x - lightWallRect.left - lightWallRect.width;
						if(isVerticallyFlipped)
							lightWallRect.top = info.tileSize.y - lightWallRect.top - lightWallRect.height;

						lightWallRect.left += tileWorldPos.x; 
						lightWallRect.top += tileWorldPos.y; 

						mRenderChunks[chunkIndex].lightWalls.emplace_back(lightWallRect);
					}

					break;
				}
			}
		}
	}

	for(size_t i = 0; i < mRenderChunks.size(); ++i)
	{
		// transform chunk bounds to world coords so we can later use them for culling in RenderSystem
		mRenderChunks[i].quadsBounds.left *= static_cast<float>(info.tileSize.x);
		mRenderChunks[i].quadsBounds.top *= static_cast<float>(info.tileSize.y);
		mRenderChunks[i].quadsBounds.width *= static_cast<float>(info.tileSize.x);
		mRenderChunks[i].quadsBounds.height *= static_cast<float>(info.tileSize.y);

		// set light walls bounds so we can do culling in RenderSystem
		mRenderChunks[i].lightWallsBounds.left = mRenderChunks[i].quadsBounds.left - 400.f;
		mRenderChunks[i].lightWallsBounds.top = mRenderChunks[i].quadsBounds.top - 400.f;
		mRenderChunks[i].lightWallsBounds.width = mRenderChunks[i].quadsBounds.width + 800.f;
		mRenderChunks[i].lightWallsBounds.height = mRenderChunks[i].quadsBounds.height + 800.f;

		// put data for static collisions optimalization
		mChunkCollisions[i].sharedBounds = mRenderChunks[i].quadsBounds;

		// put data into registry
		auto chunkEntity = mTemplates->createCopy("MapChunk", *mGameRegistry);
		auto& renderChunk = mGameRegistry->get<component::RenderChunk>(chunkEntity);
		renderChunk = mRenderChunks[i];
		auto& multiCollisionBody = mGameRegistry->get<component::MultiStaticCollisionBody>(chunkEntity);
		multiCollisionBody = mChunkCollisions[i];
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

}

