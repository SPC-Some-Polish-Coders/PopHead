#include "chunkMap.hpp"
#include "mapConstants.hpp"

namespace ph {

LayerOfChunks::LayerOfChunks(const sf::Vector2u mapSizeInTiles, const sf::Vector2u tileSizeInPixels, const sf::Texture& tileset)
	:mMapSizeInTiles(mapSizeInTiles)
	,mTileSizeInPixels(tileSizeInPixels)
{
	using namespace MapConstants;

	sf::Vector2u mapSizeInChunks(mapSizeInTiles.x / chunkSizeInTiles.x, mapSizeInTiles.y / chunkSizeInTiles.y);

	if(doesMapNotFitInChunksOnXAxis(mapSizeInChunks))
		mapSizeInChunks.x += 1;
	if(doesMapNotFitInChunksOnYAxis(mapSizeInChunks))
		mapSizeInChunks.y += 1;

	if(mapSizeInTiles.x < chunkSizeInTiles.x && mapSizeInTiles.x > 0)
		mapSizeInChunks.x = 1;
	if(mapSizeInTiles.y < chunkSizeInTiles.y && mapSizeInTiles.y > 0)
		mapSizeInChunks.y = 1;

	for(unsigned y = 0; y < mapSizeInChunks.y; ++y)
	{
		std::vector<Chunk> rowOfChunks;
		for(unsigned x = 0; x < mapSizeInChunks.x; ++x) {
			const sf::Vector2f chunkSizeInPixels(chunkSizeInTiles.x * tileSizeInPixels.x, chunkSizeInTiles.y * tileSizeInPixels.y);
			const sf::Vector2f topLeftCornerPositionInWorldOfThisChunk(y * chunkSizeInPixels.y, x * chunkSizeInPixels.x);
			rowOfChunks.emplace_back(Chunk(tileset, topLeftCornerPositionInWorldOfThisChunk, mTileSizeInPixels));
		}
		mAllChunksInLayer.emplace_back(rowOfChunks);
	}

}

bool LayerOfChunks::doesMapNotFitInChunksOnXAxis(const sf::Vector2u mapSizeInTiles)
{
	return mapSizeInTiles.x % MapConstants::chunkSizeInTiles.x != 0;
}

bool LayerOfChunks::doesMapNotFitInChunksOnYAxis(const sf::Vector2u mapSizeInTiles)
{
	return mapSizeInTiles.y % MapConstants::chunkSizeInTiles.y != 0;
}

void LayerOfChunks::addTileData(const TileData& tile)
{
	const sf::Vector2u chunkPosition = getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(tile);
	mAllChunksInLayer[chunkPosition.x][chunkPosition.y].addTileData(tile);
}

sf::Vector2u LayerOfChunks::getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(const TileData& tile)
{
	using namespace MapConstants;
	auto tilePosition = static_cast<sf::Vector2u>(tile.mTopLeftCornerPositionInWorld);
	return sf::Vector2u(
		tilePosition.x / mTileSizeInPixels.x / chunkSizeInTiles.x,
		tilePosition.y / mTileSizeInPixels.y / chunkSizeInTiles.y
	);
}

void LayerOfChunks::initializeGraphics()
{
	for(auto& chunkRow : mAllChunksInLayer)
		for(Chunk& chunk : chunkRow)
			chunk.initializeGraphics();
}

void LayerOfChunks::draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::FloatRect& cameraBounds) const
{
	for(const auto& chunkRow : mAllChunksInLayer)
		for(const Chunk& chunk : chunkRow)
			if(isChunkInCamera(chunk, cameraBounds))
				chunk.draw(target, states);
}

bool LayerOfChunks::isChunkInCamera(const Chunk& chunk, const sf::FloatRect& cameraBounds) const
{
	const sf::FloatRect chunkBounds = chunk.getGlobalBounds();
	return chunkBounds.intersects(cameraBounds);
}

ChunkMap::ChunkMap(const sf::Vector2u mapSizeInTiles, const sf::Vector2u tileSizeInPixels, const sf::Texture& tileset)
	:mMapSizeInTiles(mapSizeInTiles)
	,mTileSizeInPixels(tileSizeInPixels)
	,mTileset(tileset)
{
}

void ChunkMap::draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::FloatRect& cameraBounds) const
{
	for(const auto& layer : mLayers)
		layer.draw(target, states, cameraBounds);
}

}