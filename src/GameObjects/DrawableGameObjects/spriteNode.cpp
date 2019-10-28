#include "spriteNode.hpp"

namespace ph {

SpriteNode::SpriteNode(const Texture& texture)
	:GameObject("sprite")
	,mTexture(texture)
{
	++serialNumber;
}

void SpriteNode::drawCurrent(sf::Transform transform)
{
	Renderer::submitQuad(mTexture, getPosition(), mTexture.getSize());
}

}
