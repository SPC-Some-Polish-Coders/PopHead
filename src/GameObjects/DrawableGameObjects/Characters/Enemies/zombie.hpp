#pragma once

#include "GameObjects/DrawableGameObjects/Characters/enemy.hpp"

namespace ph {

class Zombie : public Enemy
{
public:
	Zombie(GameData*);

	void update(sf::Time delta) override;

private:
	sf::Clock timeFromLastGrowl;
};

}
