#include "spriteNode.hpp"

namespace ph {

SpriteNode::SpriteNode(const Texture& texture)
	:GameObject("sprite")
	,mSprite(texture, "sprite" + std::to_string(serialNumber))
	,mShader()
{
	++serialNumber;
	mShader.loadFromFile("resources/shaders/basic.vs.glsl", "resources/shaders/basic.fs.glsl");
}

void SpriteNode::drawCurrent(sf::Transform transform)
{
	Renderer::submit(mSprite, mShader, transform);
}

}
