#pragma once

#include "layerOfChunks.hpp"

namespace ph {

class ChunkMap : public sf::Drawable
{
public:
	explicit ChunkMap(const sf::Vector2u mapSizeInTiles, const sf::Texture& tileset);

	void addNewLayerOfChunks() { mLayers.emplace_back(mMapSizeInTiles, mTileset); };

	void addTile(const Tile& tile) { mLayers.back().addTile(tile); }

	void create() { mLayers.back().create(); }

	void draw(sf::RenderTarget& target, const sf::RenderStates states) const override;

private:
	std::vector<LayerOfChunks> mLayers;
	sf::Vector2u mMapSizeInTiles;
	const sf::Texture& mTileset;
};

}