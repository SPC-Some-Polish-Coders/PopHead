#pragma once

#include "EntityComponentSystem/Objects/character.hpp"

namespace ph {

class EnemyContainer;

class Enemy : public Character
{
public:
	Enemy(GameData* gameData, std::string name, Animation animation,
		unsigned int movementSpeed, unsigned int HP, unsigned int maxHP, sf::FloatRect posAndSize, float mass);

	friend EnemyContainer;
};

}
