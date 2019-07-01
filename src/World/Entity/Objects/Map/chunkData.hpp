#pragma once

#include <SFML/Graphics.hpp>

namespace ph {

class ChunkData
{
public:
	ChunkData(const sf::Vector2u mapSizeInTiles, const sf::Vector2u tileSizeInPixels, const sf::Texture& tileset)
		:mTileSizeInPixels(tileSizeInPixels)
		,mChunkSizeInTiles(24, 24)
		,mChunkSizeInPixels(mTileSizeInPixels.x * mChunkSizeInTiles.x, mTileSizeInPixels.y* mChunkSizeInTiles.y)
		,mMapSizeInTiles(mapSizeInTiles)
		,mTileset(tileset)
	{}

	auto getTileSizeInPixels() const -> sf::Vector2u { return mTileSizeInPixels; }
	auto getChunkSizeInTiles() const -> sf::Vector2u { return mChunkSizeInTiles; }
	auto getChunkSizeInPixels() const -> sf::Vector2u { return mChunkSizeInPixels; }
	auto getMapSizeInTiles() const -> sf::Vector2u { return mMapSizeInTiles; }
	auto getTileset() const -> const sf::Texture & { return mTileset; }

private:
	const sf::Vector2u mTileSizeInPixels;
	const sf::Vector2u mChunkSizeInTiles;
	const sf::Vector2u mChunkSizeInPixels;
	const sf::Vector2u mMapSizeInTiles;
	const sf::Texture& mTileset;
};

}