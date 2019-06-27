#pragma once

#include "layerOfChunks.hpp"

namespace ph {

class ChunkMap
{
public:
	explicit ChunkMap(const sf::Vector2u mapSizeInTiles, const sf::Texture& tileset);

	void addNewLayerOfChunks() { mLayers.emplace_back(mMapSizeInTiles, mTileset); };

	void addTile(const TileData& tile) { mLayers.back().addTile(tile); }

	void initializeGraphicsForCurrentLayer() { mLayers.back().initializeGraphics(); }

	void draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::FloatRect& cameraBounds) const;

private:
	std::vector<LayerOfChunks> mLayers;
	sf::Vector2u mMapSizeInTiles;
	const sf::Texture& mTileset;
};

}