#include "enemy.hpp"

ph::Enemy::Enemy(GameData* gameData, std::string name, Animation animation,
	         unsigned int movementSpeed, unsigned int HP, unsigned int maxHP, sf::FloatRect posAndSize, float mass)
	:Character(gameData, name, animation, movementSpeed, HP, maxHP, posAndSize, mass)
{
}
