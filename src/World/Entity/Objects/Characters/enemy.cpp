#include "enemy.hpp"

using ph::World::Entity::Enemy;

Enemy::Enemy(ph::Base::GameData* gameData, std::string name, ph::World::Animation animation,
	         unsigned int movementSpeed, unsigned int HP, unsigned int maxHP, sf::FloatRect posAndSize, float mass)
	:Character(gameData, name, animation, movementSpeed, HP, maxHP, posAndSize, mass)
{
}
