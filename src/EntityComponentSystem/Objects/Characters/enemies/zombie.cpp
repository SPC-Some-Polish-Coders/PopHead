#include "zombie.hpp"

#include "Resources/collisionRectData.hpp"
#include "gameData.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"

namespace ph {

namespace
{
	const std::string name = "zombie";
	const Animation animation;
	constexpr unsigned int movementSpeed = 100;
	constexpr unsigned int hp = 100;
	constexpr unsigned int maxHp = 100;
	const sf::FloatRect posAndSize(
		0,
		0,
		CollisionRectData::ZOMBIE_WIDTH,
		CollisionRectData::ZOMBIE_HEIGHT
	);
	constexpr float mass = 40;
}

Zombie::Zombie(GameData* gameData)
	:Enemy(gameData, name, animation, movementSpeed, hp, maxHp, posAndSize, mass)
{
	mSprite.setTexture(gameData->getTextures().get("textures/characters/zombie.png"));
}

void Zombie::update(sf::Time delta)
{
	if(timeFromLastGrowl.getElapsedTime().asSeconds() > 2) {
		mGameData->getSoundPlayer().playSpatialSound("sounds/zombieGetsAttacked.wav", mPosition);
		timeFromLastGrowl.restart();
	}

	setPosition(mCollisionBody.getPosition());
}

}