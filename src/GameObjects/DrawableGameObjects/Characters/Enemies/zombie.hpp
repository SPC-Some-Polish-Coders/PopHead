#pragma once

#include "GameObjects/DrawableGameObjects/Characters/enemy.hpp"
#include "AI/aiManager.hpp"

namespace ph {

class Zombie : public Enemy
{
public:
	Zombie(GameData*);

	void update(sf::Time delta) override;
private:
	void move();

private:
	Path mMovementPath;
	sf::Clock timeFromLastGrowl;
};

}
