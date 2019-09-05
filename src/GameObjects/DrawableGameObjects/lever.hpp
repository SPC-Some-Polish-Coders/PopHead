#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class Lever : public GameObject
{
public:
	Lever(sf::Texture& texture);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

private:
	sf::Sprite mSprite;
	bool mIsLeverDown;
};

}
