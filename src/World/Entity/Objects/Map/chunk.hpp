#pragma once

#include "SFML/Graphics.hpp"
#include <array>

namespace ph {

class Chunk
{
public:
	Chunk(const sf::Texture&, sf::Vector2f topLeftCornerPosition, const std::array<sf::IntRect, 24 * 24>& textureRects);

private:
	sf::VertexArray mVertexArray;
	const sf::Texture& mTexture;
	inline static const sf::Vector2f mChunkSize = sf::Vector2f(24.f, 24.f);
	inline static const sf::Vector2f mTileSize = sf::Vector2f(16.f, 16.f);
};

}