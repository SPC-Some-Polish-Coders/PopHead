#include "sprite.hpp"

namespace ph {

Sprite::Sprite(const Texture& texture, const std::string& name)
	:mSize(texture.getSize())
	,mTexture(texture)
{
	auto vbo = VertexBufferHolder::getInstance().getRectangleVertexBuffer(name, mSize.x, mSize.y, false);
	auto ibo = IndexBufferHolder::getInstance().getRectangleIndexBuffer();
	mVertexArray.setVertexBuffer(vbo, VertexBufferLayout::position2_texCoords2);
	mVertexArray.setIndexBuffer(ibo);
}

}
