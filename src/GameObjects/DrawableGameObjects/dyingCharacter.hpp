#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class DyingCharacter : public GameObject
{
public:
	DyingCharacter(const sf::Sprite&);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::RenderTarget&, const sf::RenderStates) const override;

private:
	sf::Sprite mSprite;
	sf::Time mTimeFromDeath;
};

}