#include "zombie.hpp"

#include "Resources/collisionRectData.hpp"

namespace
{
	const std::string name = "zombie";
	const ph::Animation animation;
	constexpr unsigned int movementSpeed = 100;
	constexpr unsigned int hp = 100;
	constexpr unsigned int maxHp = 100;
	const sf::FloatRect posAndSize(
		0,
		0,
		ph::CollisionRectData::ZOMBIE_WIDTH,
		ph::CollisionRectData::ZOMBIE_HEIGHT
	);
	constexpr float mass = 70;
}

ph::Zombie::Zombie(GameData* gameData)
	:Enemy(gameData, name, animation, movementSpeed, hp, maxHp, posAndSize, mass)
{
	mSprite.setTexture(gameData->getTextures().get("textures/characters/zombie.png"));
}

void ph::Zombie::update(sf::Time delta)
{
	if(timeFromLastGrowl.getElapsedTime().asSeconds() > 2) {
		mGameData->getSoundPlayer().playSpatialSound("sounds/zombieGetsAttacked.wav", mPosition);
		timeFromLastGrowl.restart();
	}
}
