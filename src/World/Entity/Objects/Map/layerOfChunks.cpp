#include "layerOfChunks.hpp"
#include "Utilities/debug.hpp"

namespace ph {

LayerOfChunks::LayerOfChunks(const sf::Vector2u mapSizeInTiles, const sf::Texture& tileset)
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

bool LayerOfChunks::doesMapNotFitInChunksOnXAxis(const sf::Vector2u mapSizeInTiles)
{
	return mapSizeInTiles.x % mChunkSize.x != 0;
}

bool LayerOfChunks::doesMapNotFitInChunksOnYAxis(const sf::Vector2u mapSizeInTiles)
{
	return mapSizeInTiles.y % mChunkSize.y != 0;
}

void LayerOfChunks::addTile(const Tile& tile)
{
	const sf::Vector2u chunkPosition = getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(tile);
	mChunks[chunkPosition.x][chunkPosition.y].addTile(tile);
	//TODO: Allow map size smaller then one chunk.
}

sf::Vector2u LayerOfChunks::getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(const Tile& tile)
{
	//TODO: refactor it
	const unsigned posX = tile.mTopLeftCornerPositionInWorld.x / mTileSize.x / mChunkSize.x;
	const unsigned posY = tile.mTopLeftCornerPositionInWorld.y / mTileSize.y / mChunkSize.y;

	return sf::Vector2u(
		posX, 
		posY
	);
}

void LayerOfChunks::create()
{
	int numberOfChunkWhichIsBeignCreated = 0;
	for(auto& chunkRow : mChunks)
		for(Chunk& chunk : chunkRow) {
			++numberOfChunkWhichIsBeignCreated;
			PH_LOG(LogType::Info, "chunkNumber:" + std::to_string(numberOfChunkWhichIsBeignCreated));
			chunk.create();
		}
}

void LayerOfChunks::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	for(const auto& chunkRow : mChunks)
		for(const Chunk& chunk : chunkRow)
			chunk.draw(target, states);
}

}