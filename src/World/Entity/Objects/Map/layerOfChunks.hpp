#pragma once

#include "chunk.hpp"

#include <vector>

namespace ph {

class LayerOfChunks
{
public:
	explicit LayerOfChunks(const sf::Vector2u mapSizeInTiles, const sf::Texture& tileset);
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
	sf::Vector2u mMapSizeInTiles;
};

}