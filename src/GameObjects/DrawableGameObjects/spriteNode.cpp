#include "spriteNode.hpp"

namespace ph {

SpriteNode::SpriteNode(const sf::Texture& texture)
	:GameObject("sprite")
	,mSprite(texture)
{
}

void SpriteNode::drawCurrent(sf::Transform)
{
	//target.draw(mSprite, states);
}

}
