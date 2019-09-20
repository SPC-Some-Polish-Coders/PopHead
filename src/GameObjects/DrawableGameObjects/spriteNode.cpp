#include "spriteNode.hpp"

namespace ph {

SpriteNode::SpriteNode(const sf::Texture& texture)
	:GameObject("sprite")
	,mSprite(texture)
{
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, const sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

}
