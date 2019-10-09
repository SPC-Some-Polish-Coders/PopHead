#pragma once

#include "Vertices/vertexArray.hpp"
#include "texture.hpp"

namespace ph {

// Sprite is probably a temporary helper struct, it won't be a part of ECS

struct Sprite
{
	Sprite(const Texture&, const std::string& name);

	const sf::Vector2i mSize;
	VertexArray mVertexArray;
	const Texture& mTexture;
};

}
