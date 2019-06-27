#pragma once

#include "chunk.hpp"

#include <vector>

namespace ph {

class LayerOfChunks : public sf::Drawable
{
public:
	LayerOfChunks(const sf::Vector2u mapSizeInTiles, const sf::Texture& tileset);
	bool doesMapNotFitInChunksOnXAxis(const sf::Vector2u mapSizeInTiles);
	bool doesMapNotFitInChunksOnYAxis(const sf::Vector2u mapSizeInTiles);

	void addTile(const Tile&);
	sf::Vector2u getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(const Tile&);

	void create();

	void draw(sf::RenderTarget&, const sf::RenderStates) const override;

private:
	std::vector<std::vector<Chunk>> mChunks;
	sf::Vector2u mMapSizeInTiles;
};

}