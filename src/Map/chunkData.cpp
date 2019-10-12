#include "chunkData.hpp"

namespace ph {

ChunkData::ChunkData(const sf::Vector2u mapSizeInTiles, const sf::Vector2u tileSizeInPixels, const Texture& tileset)
	:mTileSizeInPixels(tileSizeInPixels)
	,mChunkSizeInTiles(calculateChunkSizeInTiles())
	,mChunkSizeInPixels(mTileSizeInPixels.x* mChunkSizeInTiles.x, mTileSizeInPixels.y* mChunkSizeInTiles.y)
	,mMapSizeInTiles(mapSizeInTiles)
	,mTileset(tileset)
{}

auto ChunkData::calculateChunkSizeInTiles() const -> sf::Vector2u const
{
	return sf::Vector2u(
		getChunkSideSizeInTiles(mTileSizeInPixels.x),
		getChunkSideSizeInTiles(mTileSizeInPixels.y)
	);
}

unsigned ChunkData::getChunkSideSizeInTiles(unsigned tileSideSizeInPixels) const
{
	switch(tileSideSizeInPixels)
	{
	case 16: return 24;
	case 32: return 12;
	case 64: return 6;
	case 128: return 3;
	case 8: return 48;
	case 4: return 96;
	case 2: return 192;
	default: return 24;
	}
}

}