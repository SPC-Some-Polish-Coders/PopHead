#include "map.hpp"
#include "gameData.hpp"
#include "chunkMap.hpp"
#include "Utilities/debug.hpp"
#include "Utilities/csv.hpp"
#include "Utilities/math.hpp"
#include "Utilities/path.hpp"

namespace ph {

Map::Map()
	:mChunkMap(nullptr)
	,mRenderChunksMode(RenderChunksMode::forCurrentCameraView)
{
}

void Map::loadFromFile(const std::string& filename)
{
	Xml mapFile;
	mapFile.loadFromFile(filename);
	const Xml mapNode = mapFile.getChild("map");
	checkMapSupport(mapNode);
	const sf::Vector2u mapSize = getMapSize(mapNode);
	mGameData->getAIManager().registerMapSize(mapSize);
	const sf::Vector2u tileSize = getTileSize(mapNode);
	const std::vector<Xml> tilesetNodes = getTilesetNodes(mapNode);
	const TilesetsData tilesets = getTilesetsData(tilesetNodes);
	const std::vector<Xml> layerNodes = getLayerNodes(mapNode);
	
	createChunkMap(tilesets, mapSize, tileSize);
	createAllLayers(layerNodes, tilesets, mapSize, tileSize);
}

void Map::checkMapSupport(const Xml& mapNode) const
{
	const std::string orientation = mapNode.getAttribute("orientation").toString();
	if (orientation != "orthogonal")
		PH_EXCEPTION("Used unsupported map orientation: " + orientation);
	const std::string infinite = mapNode.getAttribute("infinite").toString();
	if (infinite != "0")
		PH_EXCEPTION("Infinite maps are not supported");
}

sf::Vector2u Map::getMapSize(const Xml& mapNode) const
{
	return sf::Vector2u(
		mapNode.getAttribute("width").toUnsigned(),
		mapNode.getAttribute("height").toUnsigned()
	);
}

sf::Vector2u Map::getTileSize(const Xml& mapNode) const
{
	return sf::Vector2u(
		mapNode.getAttribute("tilewidth").toUnsigned(),
		mapNode.getAttribute("tileheight").toUnsigned()
	);
}

std::vector<Xml> Map::getTilesetNodes(const Xml& mapNode) const
{
	const std::vector<Xml> tilesetNodes = mapNode.getChildren("tileset");
	if (tilesetNodes.size() == 0)
		PH_LOG(LogType::Warning, "Map doesn't have any tilesets");
	return tilesetNodes;
}

auto Map::getTilesetsData(const std::vector<Xml>& tilesetNodes) const -> const TilesetsData
{
	TilesetsData tilesets;
	tilesets.firstGlobalTileIds.reserve(tilesetNodes.size());
	tilesets.tileCounts.reserve(tilesetNodes.size());
	tilesets.columnsCounts.reserve(tilesetNodes.size());
	for (Xml tilesetNode : tilesetNodes) {
		const unsigned firstGlobalTileId = tilesetNode.getAttribute("firstgid").toUnsigned();
		tilesets.firstGlobalTileIds.push_back(firstGlobalTileId);
		if (tilesetNode.hasAttribute("source")) {
			std::string tilesetNodeSource = tilesetNode.getAttribute("source").toString();
			tilesetNodeSource = pathToMapNotEmbeddedTilesets + FilePath::toFilename(tilesetNodeSource, '/');
			PH_LOG(LogType::Info, "Detected not embeded tileset in Map: " + tilesetNodeSource);
			Xml tilesetDocument;
			tilesetDocument.loadFromFile(tilesetNodeSource);
			tilesetNode = tilesetDocument.getChild("tileset");
		}
		tilesets.tileCounts.push_back(tilesetNode.getAttribute("tilecount").toUnsigned());
		tilesets.columnsCounts.push_back(tilesetNode.getAttribute("columns").toUnsigned());
		const Xml imageNode = tilesetNode.getChild("image");
		tilesets.tilesetFileName = FilePath::toFilename(imageNode.getAttribute("source").toString(), '/');
		const std::vector<Xml> tileNodes = tilesetNode.getChildren("tile");
		TilesetsData::TilesData tilesData = getTilesData(tileNodes);
		tilesData.firstGlobalTileId = firstGlobalTileId;
		tilesets.tilesData.push_back(tilesData);
	}
	return tilesets;
}

auto Map::getTilesData(const std::vector<Xml>& tileNodes) const -> TilesetsData::TilesData
{
	TilesetsData::TilesData tilesData{};
	tilesData.ids.reserve(tileNodes.size());
	tilesData.bounds.reserve(tileNodes.size()); // WARNING: Change it when there would be possibility to have more than one CollisionBody per tile?
	for (const Xml& tileNode : tileNodes) {
		tilesData.ids.push_back(tileNode.getAttribute("id").toUnsigned());
		const Xml objectGroupNode = tileNode.getChild("objectgroup");
		const Xml objectNode = objectGroupNode.getChild("object");
		const sf::FloatRect bounds(
			objectNode.getAttribute("x").toFloat(),
			objectNode.getAttribute("y").toFloat(),
			objectNode.hasAttribute("width") ? objectNode.getAttribute("width").toFloat() : 0.f,
			objectNode.hasAttribute("height") ? objectNode.getAttribute("height").toFloat() : 0.f
		);
		if(!(bounds.width == 0.f && bounds.height == 0.f))
			tilesData.bounds.push_back(bounds);
	}
	return tilesData;
}

std::vector<Xml> Map::getLayerNodes(const Xml& mapNode) const
{
	const std::vector<Xml> layerNodes = mapNode.getChildren("layer");
	if (layerNodes.size() == 0)
		PH_LOG(LogType::Warning, "Map doesn't have any layers");
	return layerNodes;
}

void Map::createChunkMap(const TilesetsData& tilesets, const sf::Vector2u mapSize, const sf::Vector2u tileSize)
{
	mChunkMap = std::make_unique<ChunkMap>(
		mapSize,
		tileSize,
		mGameData->getTextures().get(pathToTilesetsDirectory + tilesets.tilesetFileName)
	);
}

void Map::createAllLayers(const std::vector<Xml>& layerNodes, const TilesetsData& tilesets,
                          const sf::Vector2u mapSize, const sf::Vector2u tileSize)
{
	for(const Xml& layerNode : layerNodes) {
		const Xml dataNode = layerNode.getChild("data");
		const std::vector<unsigned> globalTileIds = toGlobalTileIds(dataNode);
		createLayer(globalTileIds, tilesets, mapSize, tileSize);
	}
}

std::vector<unsigned> Map::toGlobalTileIds(const Xml& dataNode) const
{
	const std::string encoding = dataNode.getAttribute("encoding").toString();
	if (encoding == "csv")
		return Csv::toUnsigneds(dataNode.toString());
	PH_EXCEPTION("Used unsupported data encoding: " + encoding);
}

void Map::createLayer(const std::vector<unsigned>& globalTileIds, const TilesetsData& tilesets,
                      const sf::Vector2u mapSize, const sf::Vector2u tileSize)
{
	const sf::Texture& texture = mGameData->getTextures().get(pathToTilesetsDirectory + tilesets.tilesetFileName);

	mChunkMap->addNewLayerOfChunks();

	for (std::size_t tileIndexInMap = 0; tileIndexInMap < globalTileIds.size(); ++tileIndexInMap) {
		const unsigned bitsInByte = 8;
		const unsigned flippedHorizontally = 1u << (sizeof(unsigned) * bitsInByte - 1);
		const unsigned flippedVertically = 1u << (sizeof(unsigned) * bitsInByte - 2);
		const unsigned flippedDiagonally = 1u << (sizeof(unsigned) * bitsInByte - 3);

		const bool isHorizontallyFlipped = globalTileIds[tileIndexInMap] & flippedHorizontally;
		const bool isVerticallyFlipped = globalTileIds[tileIndexInMap] & flippedVertically;
		const bool isDiagonallyFlipped = globalTileIds[tileIndexInMap] & flippedDiagonally;

		const unsigned globalTileId = globalTileIds[tileIndexInMap] & (~(flippedHorizontally | flippedVertically | flippedDiagonally));

		if (hasTile(globalTileId)) {
			const std::size_t tilesetIndex = findTilesetIndex(globalTileId, tilesets);
			if (tilesetIndex == std::string::npos) {
				PH_LOG(LogType::Warning, "It was not possible to find tileset for " + std::to_string(globalTileId));
				continue;
			}
			const unsigned tileId = globalTileId - tilesets.firstGlobalTileIds[tilesetIndex];
			sf::Vector2u tileRectPosition = 
				Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(tileId, tilesets.columnsCounts[tilesetIndex]);
			tileRectPosition.x *= tileSize.x;
			tileRectPosition.y *= tileSize.y;

			sf::Vector2f position(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(tileIndexInMap, mapSize.x));
			position.x *= tileSize.x;
			position.y *= tileSize.y;

			TileData tileData;
			tileData.mTextureRectTopLeftCorner = tileRectPosition;
			tileData.mTopLeftCornerPositionInWorld = position;

			tileData.mFlipData.mIsHorizontallyFlipped = isHorizontallyFlipped;
			tileData.mFlipData.mIsVerticallyFlipped = isVerticallyFlipped;
			tileData.mFlipData.mIsDiagonallyFlipped = isDiagonallyFlipped;

			mChunkMap->addTileData(tileData);

			const std::size_t tilesDataIndex = findTilesIndex(tilesets.firstGlobalTileIds[tilesetIndex], tilesets.tilesData);
			if (tilesDataIndex == std::string::npos)
				continue;
			loadCollisionBodies(tileId, tilesets.tilesData[tilesDataIndex], position);
		}
	}

	mChunkMap->initializeGraphicsForCurrentLayer();
}

std::size_t Map::findTilesetIndex(const unsigned globalTileId, const TilesetsData& tilesets) const
{
	for (std::size_t i = 0; i < tilesets.firstGlobalTileIds.size(); ++i) {
		const unsigned firstGlobalTileId = tilesets.firstGlobalTileIds[i];
		const unsigned lastGlobalTileId = firstGlobalTileId + tilesets.tileCounts[i] - 1;
		if (globalTileId >= firstGlobalTileId && globalTileId <= lastGlobalTileId)
			return i;
	}
	return std::string::npos;
}

std::size_t Map::findTilesIndex(const unsigned firstGlobalTileId, const std::vector<TilesetsData::TilesData>& tilesData) const
{
	for (std::size_t i = 0; i < tilesData.size(); ++i)
		if (firstGlobalTileId == tilesData[i].firstGlobalTileId)
			return i;
	return std::string::npos;
}

void Map::loadCollisionBodies(const unsigned tileId, const TilesetsData::TilesData& tilesData, const sf::Vector2f position)
{
	for (std::size_t i = 0; i < tilesData.ids.size(); ++i) {
		if (tileId == tilesData.ids[i]) {
			sf::FloatRect bounds = tilesData.bounds[i];
			bounds.left += position.x;
			bounds.top += position.y;
			mGameData->getPhysicsEngine().createStaticBodyAndGetTheReference(bounds);
			mGameData->getAIManager().registerStaticCollisionBody({bounds.left, bounds.top});
		}
	}
}

void Map::draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::Vector2f cameraCenter, const sf::Vector2f cameraSize) const
{
	const sf::FloatRect screenBounds = getScreenBounds(cameraCenter, cameraSize);
	mChunkMap->draw(target, states, screenBounds);
}

auto Map::getScreenBounds(const sf::Vector2f cameraCenter, const sf::Vector2f cameraSize) const -> const sf::FloatRect
{
	const sf::Vector2f size = mRenderChunksMode == RenderChunksMode::forCurrentCameraView ? cameraSize : sf::Vector2f(640, 480);
	const sf::Vector2f topLeftCornerPosition(cameraCenter.x - size.x / 2, cameraCenter.y - size.y / 2);
	return sf::FloatRect(topLeftCornerPosition.x, topLeftCornerPosition.y, size.x, size.y);
}

}