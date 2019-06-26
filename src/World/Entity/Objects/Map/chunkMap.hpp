#pragma once

#include "chunk.hpp"

#include <vector>

namespace ph {

class ChunkMap
{
public:
	ChunkMap(const sf::Vector2u mapSizeInTiles, const sf::Texture& tileset);
	bool doesMapNotFitInChunksOnXAxis(const sf::Vector2u mapSizeInTiles);
	bool doesMapNotFitInChunksOnYAxis(const sf::Vector2u mapSizeInTiles);

	void addTile(const Tile&);
	sf::Vector2u getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(const Tile&);

	void create();

	void draw(sf::RenderTarget&, const sf::RenderStates);

private:
	std::vector<std::vector<Chunk>> mChunks;
	sf::Vector2u mMapSizeInTiles;
	const sf::Vector2u mChunkSize = sf::Vector2u(24, 24);
	const sf::Vector2f mTileSize = sf::Vector2f(16.f, 16.f);
	//TODO: Move this constant somewhere where Chunk can use it as well. Maybe separate file (mapConstants.hpp)
};

}