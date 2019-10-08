#include "spriteNode.hpp"

namespace ph {

SpriteNode::SpriteNode(const Texture& texture)
	:GameObject("sprite")
	,mSprite(texture, "sprite" + std::to_string(serialNumber))
{
	++serialNumber;
}

void SpriteNode::drawCurrent(sf::Transform transform)
{
	Renderer::submit(mSprite, transform);
}

}
