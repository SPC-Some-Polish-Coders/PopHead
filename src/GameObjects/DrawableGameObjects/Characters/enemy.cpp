#include "enemy.hpp"

namespace ph {

Enemy::Enemy(GameData* gameData, std::string name, Animation animation,
	         unsigned int movementSpeed,  int HP, unsigned int maxHP, sf::FloatRect posAndSize, float mass, unsigned int damage)
	:Character(gameData, name, animation, movementSpeed, HP, maxHP, posAndSize, mass)
{
}

}