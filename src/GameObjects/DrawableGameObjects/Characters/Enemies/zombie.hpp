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
	std::deque<Direction> mMovementPath;
	sf::Clock timeFromLastGrowl;
};

}
