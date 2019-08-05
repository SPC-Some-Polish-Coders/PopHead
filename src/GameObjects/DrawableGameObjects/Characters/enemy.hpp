#pragma once

#include "GameObjects/DrawableGameObjects/character.hpp"

namespace ph {

class EnemyContainer;

class Enemy : public Character
{
public:
	Enemy(GameData* gameData, std::string name, Animation animation,
		unsigned int movementSpeed, int HP, unsigned int maxHP, sf::FloatRect posAndSize, float mass, unsigned int damage);

	friend EnemyContainer;
};

}
