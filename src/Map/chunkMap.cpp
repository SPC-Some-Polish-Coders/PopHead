#include "chunkMap.hpp"
#include "chunkData.hpp"
#include "Utilities/rect.hpp"

namespace ph {

LayerOfChunks::LayerOfChunks(std::shared_ptr<ChunkData> chunkData)
	:mChunkData(chunkData)
{
	sf::Vector2u chunkSizeInTiles = mChunkData->getChunkSizeInTiles();
	sf::Vector2u mapSizeInTiles = mChunkData->getMapSizeInTiles();
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
			const sf::Vector2f chunkSizeInPixels = static_cast<sf::Vector2f>(mChunkData->getChunkSizeInPixels());
			const sf::Vector2f topLeftCornerPositionInWorldOfThisChunk(x * chunkSizeInPixels.x, y * chunkSizeInPixels.y);
			rowOfChunks.emplace_back(Chunk(topLeftCornerPositionInWorldOfThisChunk, mChunkData));
		}
		mAllChunksInLayer.emplace_back(rowOfChunks);
	}
}

bool LayerOfChunks::doesMapNotFitInChunksOnXAxis(const sf::Vector2u mapSizeInTiles)
{
	return mapSizeInTiles.x % mChunkData->getChunkSizeInTiles().x != 0;
}

bool LayerOfChunks::doesMapNotFitInChunksOnYAxis(const sf::Vector2u mapSizeInTiles)
{
	return mapSizeInTiles.y % mChunkData->getChunkSizeInTiles().y != 0;
}

void LayerOfChunks::addTileData(const TileData& tile)
{
	const sf::Vector2u chunkPosition = getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(tile);
	mAllChunksInLayer[chunkPosition.y][chunkPosition.x].addTileData(tile);
}

sf::Vector2u LayerOfChunks::getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(const TileData& tile)
{
	auto tilePosition = static_cast<sf::Vector2u>(tile.mTopLeftCornerPositionInWorld);
	return sf::Vector2u(
		tilePosition.x / mChunkData->getTileSizeInPixels().x / mChunkData->getChunkSizeInTiles().x,
		tilePosition.y / mChunkData->getTileSizeInPixels().y / mChunkData->getChunkSizeInTiles().y
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
	const FloatRect chunkBounds = chunk.getGlobalBounds();
	return chunkBounds.doPositiveRectsIntersect(cameraBounds);
}

ChunkMap::ChunkMap(const sf::Vector2u mapSizeInTiles, const sf::Vector2u tileSizeInPixels, const sf::Texture& tileset)
	:mChunkData(new ChunkData(mapSizeInTiles, tileSizeInPixels, tileset))
{
}

void ChunkMap::addNewLayerOfChunks()
{
	mLayers.emplace_back(mChunkData);
}

void ChunkMap::addTileData(const TileData& tile)
{
	mLayers.back().addTileData(tile);
}

void ChunkMap::initializeGraphicsForCurrentLayer()
{
	mLayers.back().initializeGraphics();
}

void ChunkMap::draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::FloatRect& cameraBounds) const
{
	for(const auto& layer : mLayers)
		layer.draw(target, states, cameraBounds);
}

}