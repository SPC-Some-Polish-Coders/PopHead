#include "zombie.hpp"

using PopHead::World::Entity::Zombie;
using PopHead::World::Animation;

namespace
{
	const std::string name = "zombie";
	const Animation animation = Animation();
	constexpr unsigned int movementSpeed = 100;
	constexpr unsigned int hp = 100;
	constexpr unsigned int maxHp = 100;
	const sf::FloatRect posAndSize = sf::FloatRect(0, 0, 32, 32); //TODO: Make zombie collision rect in Resources/CollisionRectData.hpp
	constexpr float mass = 70;
}

Zombie::Zombie(PopHead::Base::GameData* gameData)
	:Enemy(gameData, name, animation, movementSpeed, hp, maxHp, posAndSize, mass)
{
}
