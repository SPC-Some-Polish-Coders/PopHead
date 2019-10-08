#pragma once

#include "Vertices/vertexArray.hpp"
#include "texture.hpp"

namespace ph {

struct Sprite
{
	Sprite(const Texture&, const std::string& name);

	VertexArray mVertexArray;
	const Texture& mTexture;
};

}
