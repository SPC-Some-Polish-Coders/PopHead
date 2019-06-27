#include "layerOfChunks.hpp"
#include "mapConstants.hpp"
#include "Utilities/debug.hpp"

namespace ph {

LayerOfChunks::LayerOfChunks(const sf::Vector2u mapSizeInTiles, const sf::Texture& tileset)
	:mMapSizeInTiles(mapSizeInTiles)
{
	using namespace MapConstants;

	sf::Vector2u mapSizeInChunks(mapSizeInTiles.x / mChunkSize.x, mapSizeInTiles.y / mChunkSize.y);

	if(doesMapNotFitInChunksOnXAxis(mapSizeInChunks))
		mapSizeInChunks.x += 1;
	if(doesMapNotFitInChunksOnYAxis(mapSizeInChunks))
		mapSizeInChunks.y += 1;

	if(mapSizeInTiles.x < mChunkSize.x && mapSizeInTiles.x > 0)
		mapSizeInChunks.x = 1;
	if(mapSizeInTiles.y < mChunkSize.y && mapSizeInTiles.y > 0)
		mapSizeInChunks.y = 1;

	std::vector<Chunk> rowOfChunks;
	for(unsigned x = 0; x < mapSizeInChunks.x; ++x)
		rowOfChunks.emplace_back(Chunk(tileset));

	for(unsigned y = 0; y < mapSizeInChunks.y; ++y)
		mChunks.emplace_back(rowOfChunks);
}

bool LayerOfChunks::doesMapNotFitInChunksOnXAxis(const sf::Vector2u mapSizeInTiles)
{
	return mapSizeInTiles.x % MapConstants::mChunkSize.x != 0;
}

bool LayerOfChunks::doesMapNotFitInChunksOnYAxis(const sf::Vector2u mapSizeInTiles)
{
	return mapSizeInTiles.y % MapConstants::mChunkSize.y != 0;
}

void LayerOfChunks::addTile(const TileData& tile)
{
	const sf::Vector2u chunkPosition = getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(tile);
	mChunks[chunkPosition.x][chunkPosition.y].addTile(tile);
}

sf::Vector2u LayerOfChunks::getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(const TileData& tile)
{
	using namespace MapConstants;
	auto tilePosition = static_cast<sf::Vector2u>(tile.mTopLeftCornerPositionInWorld);
	return sf::Vector2u(
		tilePosition.x / mTileSize.x / mChunkSize.x,
		tilePosition.y / mTileSize.y / mChunkSize.y
	);
}

void LayerOfChunks::initializeGraphics()
{
	for(auto& chunkRow : mChunks)
		for(Chunk& chunk : chunkRow)
			chunk.initializeGraphics();
}

void LayerOfChunks::draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::FloatRect& cameraBounds) const
{
	for(const auto& chunkRow : mChunks)
		for(const Chunk& chunk : chunkRow)
			if(isChunkInCamera(chunk, cameraBounds))
				chunk.draw(target, states);
}

bool LayerOfChunks::isChunkInCamera(const Chunk& chunk, const sf::FloatRect& cameraBounds) const
{
	const sf::FloatRect chunkBounds = chunk.getGlobalBounds();
	return chunkBounds.intersects(cameraBounds);
}

}