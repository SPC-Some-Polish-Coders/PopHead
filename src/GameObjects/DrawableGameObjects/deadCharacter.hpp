#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class DeadCharacter : public GameObject
{
public:
	DeadCharacter(const sf::Sprite&);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::Transform) override;

private:
	sf::Sprite mSprite;
	sf::Time mTimeFromDeath;
};

}