#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class Lever : public GameObject
{
public:
	Lever(sf::Texture& leverTexture, sf::Texture& hintTexture);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::RenderTarget&, sf::RenderStates) const override;

private:
	sf::Sprite mLeverSprite;
	sf::Sprite mHintSprite;
	bool mIsLeverDown;
	bool mIsPlayerInHintArea;
};

}
