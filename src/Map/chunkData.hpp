#pragma once

#include <SFML/Graphics.hpp>
#include "Renderer/texture.hpp"

namespace ph {

class ChunkData
{
public:
	ChunkData(const sf::Vector2u mapSizeInTiles, const sf::Vector2u tileSizeInPixels, const Texture& tileset);

	auto getTileSizeInPixels() const -> sf::Vector2u { return mTileSizeInPixels; }
	auto getChunkSizeInTiles() const -> sf::Vector2u { return mChunkSizeInTiles; }
	auto getChunkSizeInPixels() const -> sf::Vector2u { return mChunkSizeInPixels; }
	auto getMapSizeInTiles() const -> sf::Vector2u { return mMapSizeInTiles; }
	auto getTileset() const -> const Texture & { return mTileset; }

private:
	 auto calculateChunkSizeInTiles() const -> sf::Vector2u const;
	 unsigned getChunkSideSizeInTiles(unsigned tileSideSizeInPixels) const;

private:
	const sf::Vector2u mTileSizeInPixels;
	const sf::Vector2u mChunkSizeInTiles;
	const sf::Vector2u mChunkSizeInPixels;
	const sf::Vector2u mMapSizeInTiles;
	const Texture& mTileset;
};

}