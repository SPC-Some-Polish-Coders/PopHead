#include "xmlMapParser.hpp"
#include "Logs/logs.hpp"
#include "Utilities/profiling.hpp"
#include "AI/aiManager.hpp"
#include "Utilities/xml.hpp"
#include "Utilities/csv.hpp"
#include "Utilities/filePath.hpp"
#include "Utilities/math.hpp"

namespace ph {

void XmlMapParser::parseFile(const Xml& mapNode, AIManager& aiManager, entt::registry& gameRegistry, EntitiesTemplateStorage& templates)
{
	mGameRegistry = &gameRegistry;
	mTemplates = &templates;

	GeneralMapInfo info = getGeneralMapInfo(mapNode);
	aiManager.registerMapSize(info.mapSize);

	mRenderChunks.resize(static_cast<size_t>(info.nrOfChunks));
	mChunkCollisions.resize(static_cast<size_t>(info.nrOfChunks));

	const std::vector<Xml> tilesetNodes = mapNode.getChildren("tileset");
	PH_ASSERT_WARNING(tilesetNodes.size() != 0, "Map doesn't have any tilesets");

	const TilesetsData tilesetsData = getTilesetsData(tilesetNodes);
	const std::vector<Xml> layerNodes = getLayerNodes(mapNode);
	
	parserMapLayers(layerNodes, tilesetsData, info, aiManager);
	createMapBorders(info);
}

auto XmlMapParser::getGeneralMapInfo(const Xml& mapNode) const -> GeneralMapInfo
{
	const std::string orientation = mapNode.getAttribute("orientation")->toString();
	PH_ASSERT_CRITICAL(orientation == "orthogonal", "Used unsupported map orientation: " + orientation);

	GeneralMapInfo info;
	info.mapSize.x = mapNode.getAttribute("width")->toUnsigned();
	info.mapSize.y = mapNode.getAttribute("height")->toUnsigned();
	info.tileSize.x = mapNode.getAttribute("tilewidth")->toUnsigned();
	info.tileSize.y = mapNode.getAttribute("tileheight")->toUnsigned();
	info.isInfinite = mapNode.getAttribute("infinite")->toBool();
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
	
	for(Xml tilesetNode : tilesetNodes) {
		const unsigned firstGlobalTileId = tilesetNode.getAttribute("firstgid")->toUnsigned();
		tilesets.firstGlobalTileIds.push_back(firstGlobalTileId);
		if(auto source = tilesetNode.getAttribute("source")) {
			std::string tilesetNodeSource = source->toString();
			tilesetNodeSource = FilePath::toFilename(tilesetNodeSource, '/');
			PH_LOG_INFO("Detected not embedded tileset in Map: " + tilesetNodeSource);
			Xml tilesetDocument;
			PH_ASSERT_CRITICAL(tilesetDocument.loadFromFile(tilesetNodeSource),
				"Not embedded tileset file \"" + tilesetNodeSource + "\" wasn't loaded correctly!");
			tilesetNode = *tilesetDocument.getChild("tileset");
		}

		tilesets.tileCounts.push_back(tilesetNode.getAttribute("tilecount")->toUnsigned());
		tilesets.columnsCounts.push_back(tilesetNode.getAttribute("columns")->toUnsigned());
		const Xml imageNode = *tilesetNode.getChild("image");
		tilesets.tilesetFileName = FilePath::toFilename(imageNode.getAttribute("source")->toString(), '/');
		const std::vector<Xml> tileNodes = tilesetNode.getChildren("tile");
		TilesData tilesData = getTilesData(tileNodes);
		tilesData.firstGlobalTileId = firstGlobalTileId;
		tilesets.tilesData.push_back(tilesData);
	}

	return tilesets;
}

auto XmlMapParser::getTilesData(const std::vector<Xml>& tileNodes) const -> TilesData
{
	TilesData tilesData{};
	tilesData.ids.reserve(tileNodes.size());
	tilesData.bounds.reserve(tileNodes.size());
	for(const Xml& tileNode : tileNodes) {
		tilesData.ids.push_back(tileNode.getAttribute("id")->toUnsigned());
		const auto objectGroupNode = tileNode.getChild("objectgroup");
		const Xml objectNode = *objectGroupNode->getChild("object");
		auto width = objectNode.getAttribute("width");
		auto height = objectNode.getAttribute("height");
		const sf::FloatRect bounds(
			objectNode.getAttribute("x")->toFloat(),
			objectNode.getAttribute("y")->toFloat(),
			width ? width->toFloat() : 0.f,
			height ? height->toFloat() : 0.f
		);
		tilesData.bounds.push_back(bounds);
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

void XmlMapParser::parserMapLayers(const std::vector<Xml>& layerNodes, const TilesetsData& tilesets, const GeneralMapInfo& info,
                                   AIManager& aiManager)
{
	unsigned char z = sLowestLayerZ;
	for (const Xml& layerNode : layerNodes)
	{
		const Xml dataNode = *layerNode.getChild("data");
		const auto globalIds = toGlobalTileIds(dataNode);
		createLayer(globalIds, tilesets, info, z, aiManager);
		--z;
	}
}

std::vector<unsigned> XmlMapParser::toGlobalTileIds(const Xml& dataNode) const
{
	const std::string encoding = dataNode.getAttribute("encoding")->toString();
	if(encoding == "csv")
		return Csv::toUnsigneds(dataNode.toString());
	PH_EXIT_GAME("Used unsupported data encoding: " + encoding);
}

void XmlMapParser::createLayer(const std::vector<unsigned>& globalTileIds, const TilesetsData& tilesets,
                               const GeneralMapInfo& info, unsigned char z, AIManager& aiManager)
{
	PH_PROFILE_FUNCTION(0);

	// clear chunks 
	bool isFirstLayer = z == sLowestLayerZ;
	if(!isFirstLayer)
	{
		for(size_t i = 0; i < mRenderChunks.size(); ++i) {
			mRenderChunks[i] = component::RenderChunk();
			mChunkCollisions[i] = component::MultiStaticCollisionBody();	
		}
	}

	// fill chunks them with z and bounds
	for(size_t i = 0; i < mRenderChunks.size(); ++i)
	{
		mRenderChunks[i].z = z;

		float row = std::floor(i / info.nrOfChunksInOneRow);
		mRenderChunks[i].bounds = sf::FloatRect((sChunkSize * i) - (row * sChunkSize * info.nrOfChunksInOneRow), row * sChunkSize, sChunkSize, sChunkSize);
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
			const std::size_t tilesetIndex = findTilesetIndex(globalTileId, tilesets);
			if (tilesetIndex == std::string::npos) {
				PH_LOG_WARNING("It was not possible to find tileset for " + std::to_string(globalTileId));
				continue;
			}

			sf::Vector2f positionInTiles(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(tileIndexInMap, info.mapSize.x));

			// create quad data
			QuadData qd;

			qd.position = sf::Vector2f(
				positionInTiles.x * static_cast<float>(info.tileSize.x),
				positionInTiles.y * static_cast<float>(info.tileSize.y));

			auto tileSize = static_cast<sf::Vector2f>(info.tileSize);
			qd.rotationOrigin = {tileSize.x / 2.f, tileSize.y / 2.f};
			if(!(isHorizontallyFlipped || isVerticallyFlipped || isDiagonallyFlipped)) {
				qd.size = tileSize;
				qd.rotation = 0.f;
			}
			else if(isHorizontallyFlipped && isVerticallyFlipped && isDiagonallyFlipped) {
				qd.size = {tileSize.x, -tileSize.y};
				qd.position.x += tileSize.x;
				qd.rotation = 270.f;
			}
			else if(isHorizontallyFlipped && isVerticallyFlipped) {
				qd.size = -tileSize;
				qd.position += tileSize;
				qd.rotation = 0.f;
			}
			else if(isHorizontallyFlipped && isDiagonallyFlipped) {
				qd.size = tileSize;
				qd.rotation = 90.f;
			}
			else if(isVerticallyFlipped && isDiagonallyFlipped) {
				qd.size = tileSize;
				qd.rotation = 270.f;
			}
			else if(isHorizontallyFlipped) {
				qd.size = {-tileSize.x, tileSize.y};
				qd.position.x += tileSize.x;
				qd.rotation = 0.f;
			}
			else if(isVerticallyFlipped) {
				qd.size = {tileSize.x, -tileSize.y};
				qd.position.y += tileSize.y;
				qd.rotation = 0.f;
			}
			else if(isDiagonallyFlipped) {
				qd.size = {-tileSize.x, tileSize.y};
				qd.position.y -= tileSize.x;
				qd.rotation = 270.f;
			}
			qd.rotation = Math::degreesToRadians(qd.rotation);

			qd.color = Vector4f{1.f, 1.f, 1.f, 1.f};
			qd.textureSlotRef = 0.f;

			const unsigned tileId = globalTileId - tilesets.firstGlobalTileIds[tilesetIndex];
			auto tileRectPosition = static_cast<sf::Vector2f>(
				Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(tileId, tilesets.columnsCounts[tilesetIndex]));
			tileRectPosition.x *= (info.tileSize.x + 2);
			tileRectPosition.y *= (info.tileSize.y + 2);
			tileRectPosition.x += 1;
			tileRectPosition.y += 1;
			const sf::Vector2f textureSize(576.f, 576.f); // TODO: Make it not hardcoded like that
			qd.textureRect.left = tileRectPosition.x / textureSize.x;
			qd.textureRect.top = (textureSize.y - tileRectPosition.y - info.tileSize.y) / textureSize.y;
			qd.textureRect.width = static_cast<float>(info.tileSize.x) / textureSize.x;
			qd.textureRect.height = static_cast<float>(info.tileSize.y) / textureSize.y;

			// TODO: Optimize that
			// find chunk index
			size_t chunkIndex = 0;
			for(size_t i = 0; i < mRenderChunks.size(); ++i)
				if(mRenderChunks[i].bounds.containsIncludingBounds(positionInTiles))
					chunkIndex = i;

			// emplace quad data to chunk
			mRenderChunks[chunkIndex].quads.emplace_back(qd);

			// load collision bodies
			const std::size_t tilesDataIndex = findTilesIndex(tilesets.firstGlobalTileIds[tilesetIndex], tilesets.tilesData);
			if (tilesDataIndex == std::string::npos)
				continue;

			auto& tilesData = tilesets.tilesData[tilesDataIndex];
			for (std::size_t i = 0; i < tilesData.ids.size(); ++i) {
				if (tileId == tilesData.ids[i]) {
					sf::FloatRect bounds = tilesData.bounds[i];
					bounds.left += qd.position.x;
					bounds.top += qd.position.y;
					aiManager.registerObstacle({bounds.left, bounds.top});
					mChunkCollisions[chunkIndex].rects.emplace_back(bounds);
				}
			}
		}
	}

	for(size_t i = 0; i < mRenderChunks.size(); ++i)
	{
		// transform chunk bounds to world coords so we can later use them for culling in RenderSystem
		mRenderChunks[i].bounds.left *= static_cast<float>(info.tileSize.x);
		mRenderChunks[i].bounds.top *= static_cast<float>(info.tileSize.y);
		mRenderChunks[i].bounds.width *= static_cast<float>(info.tileSize.x);
		mRenderChunks[i].bounds.height *= static_cast<float>(info.tileSize.y);

		// put data for static collisions optimalization
		mChunkCollisions[i].sharedBounds = mRenderChunks[i].bounds;

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
	for (std::size_t i = 0; i < tilesets.firstGlobalTileIds.size(); ++i) {
		const unsigned firstGlobalTileId = tilesets.firstGlobalTileIds[i];
		const unsigned lastGlobalTileId = firstGlobalTileId + tilesets.tileCounts[i] - 1;
		if (globalTileId >= firstGlobalTileId && globalTileId <= lastGlobalTileId)
			return i;
	}
	return std::string::npos;
}

std::size_t XmlMapParser::findTilesIndex(const unsigned firstGlobalTileId, const std::vector<TilesData>& tilesData) const
{
	for (std::size_t i = 0; i < tilesData.size(); ++i)
		if (firstGlobalTileId == tilesData[i].firstGlobalTileId)
			return i;
	return std::string::npos;
}

void XmlMapParser::createMapBorders(const GeneralMapInfo& mapInfo)
{
	auto mapWidth = static_cast<float>(mapInfo.mapSize.x * mapInfo.tileSize.x);
	auto mapHeight = static_cast<float>(mapInfo.mapSize.y * mapInfo.tileSize.y);

	const sf::Vector2f tileSize(sf::Vector2u(mapInfo.tileSize.x, mapInfo.tileSize.y));
	
	// create top border
	auto topBorderEntity = mTemplates->createCopy("BorderCollision", *mGameRegistry);
	auto& topBody = mGameRegistry->get<component::BodyRect>(topBorderEntity);
	topBody.rect = FloatRect(-tileSize.x, -tileSize.y, mapWidth + 2 * tileSize.x, tileSize.y);

	// create bottom border
	auto bottomBorderEntity = mTemplates->createCopy("BorderCollision", *mGameRegistry);
	auto& bottomBody = mGameRegistry->get<component::BodyRect>(bottomBorderEntity);
	bottomBody.rect = FloatRect(-tileSize.x, mapHeight, mapWidth + 2 * tileSize.x, tileSize.y);
		
	// left border
	auto leftborderEntity = mTemplates->createCopy("BorderCollision", *mGameRegistry);
	auto& leftBody = mGameRegistry->get<component::BodyRect>(leftborderEntity);
	leftBody.rect = FloatRect(-tileSize.x, -tileSize.y, tileSize.x, mapHeight + 2 * tileSize.y);

	// right border
	auto rightBorderEntity = mTemplates->createCopy("BorderCollision", *mGameRegistry);
	auto& rightBody = mGameRegistry->get<component::BodyRect>(rightBorderEntity);
	rightBody.rect = FloatRect(mapWidth, -tileSize.y, tileSize.x, mapHeight + 2 * tileSize.y);
}

}
