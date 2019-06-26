#include "chunk.hpp"

namespace ph {

Chunk::Chunk(const sf::Texture&, sf::Vector2f topLeftCornerPosition, const std::array<sf::IntRect, 24 * 24>& textureRects)
	:mTexture(texture)
{
	mVertexArray.setPrimitiveType(sf::Quads);
	mVertexArray.resize(mChunkSize.x * mChunkSize.y * 4);

}

}