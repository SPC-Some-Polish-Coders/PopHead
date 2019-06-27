#include "chunkMap.hpp"
#include "Utilities/debug.hpp"

namespace ph {

ChunkMap::ChunkMap(const sf::Vector2u mapSizeInTiles, const sf::Texture& tileset)
	:mMapSizeInTiles(mapSizeInTiles)
{
	sf::Vector2u mapSizeInChunks(mapSizeInTiles.x / mChunkSize.x, mapSizeInTiles.y / mChunkSize.y);
	if(doesMapNotFitInChunksOnXAxis(mapSizeInChunks))
		mapSizeInChunks.x += 1;
	if(doesMapNotFitInChunksOnYAxis(mapSizeInChunks))
		mapSizeInChunks.y += 1;

	std::vector<Chunk> rowOfChunks;
	for(int x = 0; x < mapSizeInChunks.x; ++x)
		rowOfChunks.emplace_back(Chunk(tileset));

	for(int y = 0; y < mapSizeInChunks.y; ++y)
		mChunks.emplace_back(rowOfChunks);
}

bool ChunkMap::doesMapNotFitInChunksOnXAxis(const sf::Vector2u mapSizeInTiles)
{
	return mapSizeInTiles.x % mChunkSize.x != 0;
}

bool ChunkMap::doesMapNotFitInChunksOnYAxis(const sf::Vector2u mapSizeInTiles)
{
	return mapSizeInTiles.y % mChunkSize.y != 0;
}

void ChunkMap::addTile(const Tile& tile)
{
	const sf::Vector2u chunkPosition = getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(tile);
	mChunks[chunkPosition.x][chunkPosition.y].addTile(tile);
}

sf::Vector2u ChunkMap::getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(const Tile& tile)
{
	const unsigned posX = tile.mTopLeftCornerPositionInWorld.x / mTileSize.x / mChunkSize.x;
	const unsigned posY = tile.mTopLeftCornerPositionInWorld.y / mTileSize.y / mChunkSize.y;

	return sf::Vector2u(
		posX, 
		posY
	);
}

void ChunkMap::create()
{
	int numberOfChunkWhichIsBeignCreated = 0;
	for(auto& chunkRow : mChunks)
		for(Chunk& chunk : chunkRow) {
			++numberOfChunkWhichIsBeignCreated;
			PH_LOG(LogType::Info, "chunkNumber:" + std::to_string(numberOfChunkWhichIsBeignCreated));
			chunk.create();
		}
}

void ChunkMap::draw(sf::RenderTarget& target, const sf::RenderStates states)
{
	for(const auto& chunkRow : mChunks)
		for(const Chunk& chunk : chunkRow)
			chunk.draw(target, states);
}

}