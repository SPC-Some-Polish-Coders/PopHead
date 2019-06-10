#include "enemy.hpp"

using PopHead::World::Entity::Enemy;

Enemy::Enemy(PopHead::Base::GameData* gameData, std::string name, PopHead::World::Animation animation,
	         unsigned int movementSpeed, unsigned int HP, unsigned int maxHP, sf::FloatRect posAndSize, float mass)
	:Character(gameData, name, animation, movementSpeed, HP, maxHP, posAndSize, mass)
{
}
