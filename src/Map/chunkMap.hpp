#pragma once

#include "chunk.hpp"
#include "chunkData.hpp"
#include <vector>
#include <memory>

namespace ph {

class LayerOfChunks
{
public:
	explicit LayerOfChunks(std::shared_ptr<ChunkData>);
	bool doesMapNotFitInChunksOnXAxis(const sf::Vector2u mapSizeInTiles);
	bool doesMapNotFitInChunksOnYAxis(const sf::Vector2u mapSizeInTiles);

	void addTileData(const TileData&);
	sf::Vector2u getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(const TileData&);

	void initializeGraphics();

	void draw(sf::RenderTarget&, const sf::RenderStates, const sf::FloatRect&) const;
private:
	bool isChunkInCamera(const Chunk& chunk, const sf::FloatRect& cameraBounds) const;

private:
	using RowOfChunks = std::vector<Chunk>;
	using ChunkMatrix = std::vector<RowOfChunks>;
	ChunkMatrix mAllChunksInLayer;
	std::shared_ptr<ChunkData> mChunkData;
};

class ChunkMap
{
public:
	explicit ChunkMap(const sf::Vector2u mapSizeInTiles, const sf::Vector2u tileSizeInPixels, const sf::Texture& tileset);

	void addNewLayerOfChunks();

	void addTileData(const TileData& tile);

	void initializeGraphicsForCurrentLayer();

	void draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::FloatRect& cameraBounds) const;

private:
	std::vector<LayerOfChunks> mLayers;
	std::shared_ptr<ChunkData> mChunkData;
};

}