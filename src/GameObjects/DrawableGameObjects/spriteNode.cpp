#include "spriteNode.hpp"

namespace ph {

SpriteNode::SpriteNode(const Texture& texture)
	:GameObject("sprite")
	,mVertexArray(std::make_shared<VertexArray>())
	,mShader(std::make_shared<Shader>())
	,mTexture(texture)
{
	auto vbo = VertexBufferHolder::getGlobalInstance().getRectangleVertexBuffer("sprite", texture.getWidth(), texture.getHeight());
	auto ibo = IndexBufferHolder::getGlobalInstance().getRectangleIndexBuffer("rectangle");
	mVertexArray->setVertexBuffer(vbo, VertexBufferLayout::position2_texCoords2);
	mVertexArray->setIndexBuffer(ibo);

	mShader->loadFromFile("resources/shaders/basic.vs.glsl", "resources/shaders/basic.fs.glsl");
}

void SpriteNode::drawCurrent(sf::Transform transform)
{
	mTexture.bind();
	Renderer::submit(mVertexArray, mShader, transform);
}

}
