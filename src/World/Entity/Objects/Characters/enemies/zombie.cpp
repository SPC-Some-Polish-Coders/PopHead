#include "zombie.hpp"

#include "Resources/collisionRectData.hpp"

using PopHead::World::Entity::Zombie;
using PopHead::World::Animation;

namespace
{
	using namespace PopHead::Resources;

	const std::string name = "zombie";
	const Animation animation = Animation();
	constexpr unsigned int movementSpeed = 100;
	constexpr unsigned int hp = 100;
	constexpr unsigned int maxHp = 100;
	const sf::FloatRect posAndSize = sf::FloatRect(0, 0, CollisionRectData::ZOMBIE_WIDTH, CollisionRectData::ZOMBIE_HEIGHT);
	constexpr float mass = 70;
}

Zombie::Zombie(PopHead::Base::GameData* gameData)
	:Enemy(gameData, name, animation, movementSpeed, hp, maxHp, posAndSize, mass)
{
	mSprite.setTexture(gameData->getTextures().get("resources/textures/characters/zombie.png"));
}

void Zombie::update(sf::Time delta)
{
	if(timeFromLastGrowl.getElapsedTime().asSeconds() > 2) {
		mGameData->getSoundPlayer().playSpatialSound("resources/sounds/zombieGetsAttacked.wav", mPosition);
		timeFromLastGrowl.restart();
	}
}
