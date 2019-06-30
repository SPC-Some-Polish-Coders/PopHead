#include "chunkMap.hpp"

namespace ph {

ChunkMap::ChunkMap(const sf::Vector2u mapSizeInTiles, const sf::Texture& tileset)
	:mMapSizeInTiles(mapSizeInTiles)
	,mTileset(tileset)
{
}

void ChunkMap::draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::FloatRect& cameraBounds) const
{
	for(const auto& layer : mLayers)
		layer.draw(target, states, cameraBounds);
}

}