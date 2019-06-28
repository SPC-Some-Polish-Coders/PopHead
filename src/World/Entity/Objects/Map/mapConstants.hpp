#pragma once

#include <SFML/Graphics.hpp>

namespace ph {

namespace MapConstants
{
	const sf::Vector2u mChunkSizeInTiles(24, 24);
	const sf::Vector2u mChunkSizeInPixels(384, 384);
	const sf::Vector2u mTileSizeInPixels(16, 16);
	//TODO: Make tile size not fixed. Making 32x32 or 64x64 tile maps should be possible as well
}

}