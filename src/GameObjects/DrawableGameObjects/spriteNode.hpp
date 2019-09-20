#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class SpriteNode : public GameObject
{
public:
	SpriteNode(const sf::Texture&);

	void drawCurrent(sf::RenderTarget&, const sf::RenderStates) const override;
	sf::Sprite& getSprite() { return mSprite; }
private:
	sf::Sprite mSprite;
};

}