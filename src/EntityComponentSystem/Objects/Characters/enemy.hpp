#pragma once

#include "EntityComponentSystem/Objects/character.hpp"
#include <set>

namespace ph {

class Enemy : public Character
{
public:
	Enemy(GameData* gameData, std::string name, Animation animation,
		unsigned int movementSpeed, unsigned int HP, unsigned int maxHP, sf::FloatRect posAndSize, float mass);
};

}
