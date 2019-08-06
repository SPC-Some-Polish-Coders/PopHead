#include "map.hpp"
#include "chunkMap.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

Map::Map()
	:mChunkMap(nullptr)
{
}

void Map::load(const GeneralMapInfo& info, const TilesetsData& tilesetsData, const AllLayersGlobalTileIds& allLayersGlobalTileIds)
{
	createChunkMap(tilesetsData, info);
	createAllLayers(allLayersGlobalTileIds, tilesetsData, info);
}

void Map::createChunkMap(const TilesetsData& tilesetsData, const GeneralMapInfo& info)
{
	mChunkMap = std::make_unique<ChunkMap>(
		info.mapSize,
		info.tileSize,
		mGameData->getTextures().get(pathToTilesetsDirectory + tilesetsData.tilesetFileName)
	);
}

void Map::createAllLayers(const AllLayersGlobalTileIds& allLayers, const TilesetsData& tilesets, const GeneralMapInfo& info)
{
	for(const GlobalTileIds& globalTileIds : allLayers)
		createLayer(globalTileIds, tilesets, info);
}

void Map::createLayer(const std::vector<unsigned>& globalTileIds, const TilesetsData& tilesets, const GeneralMapInfo& info)
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
				PH_LOG_WARNING("It was not possible to find tileset for " + std::to_string(globalTileId));
				continue;
			}
			const unsigned tileId = globalTileId - tilesets.firstGlobalTileIds[tilesetIndex];
			sf::Vector2u tileRectPosition = 
				Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(tileId, tilesets.columnsCounts[tilesetIndex]);
			tileRectPosition.x *= info.tileSize.x;
			tileRectPosition.y *= info.tileSize.y;

			sf::Vector2f position(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(tileIndexInMap, info.mapSize.x));
			position.x *= info.tileSize.x;
			position.y *= info.tileSize.y;

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

std::size_t Map::findTilesIndex(const unsigned firstGlobalTileId, const std::vector<TilesData>& tilesData) const
{
	for (std::size_t i = 0; i < tilesData.size(); ++i)
		if (firstGlobalTileId == tilesData[i].firstGlobalTileId)
			return i;
	return std::string::npos;
}

void Map::loadCollisionBodies(const unsigned tileId, const TilesData& tilesData, const sf::Vector2f position)
{
	for (std::size_t i = 0; i < tilesData.ids.size(); ++i) {
		if (tileId == tilesData.ids[i]) {
			sf::FloatRect bounds = tilesData.bounds[i];
			bounds.left += position.x;
			bounds.top += position.y;
			mGameData->getPhysicsEngine().createStaticBodyAndGetTheReference(bounds);
			mGameData->getAIManager().registerObstacle({bounds.left, bounds.top});
		}
	}
}

void Map::draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::FloatRect cameraBounds) const
{
	mChunkMap->draw(target, states, cameraBounds);
}

}