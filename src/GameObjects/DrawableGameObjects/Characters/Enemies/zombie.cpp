#include "zombie.hpp"
#include "gameData.hpp"
#include "Resources/collisionRectData.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "GameObjects/GameObjectContainers/enemyContainer.hpp"

namespace ph {

namespace
{
	const std::string name = "zombie";
	const Animation animation;
	constexpr float movementSpeed = 50.f;
	constexpr int hp = 100;
	constexpr unsigned maxHp = 100;
	constexpr unsigned damage = 20;
	const sf::FloatRect posAndSize(
		0,
		0,
		CollisionRectData::ZOMBIE_WIDTH,
		CollisionRectData::ZOMBIE_HEIGHT
	);
	constexpr float mass = 40;
}

Zombie::Zombie(GameData* gameData)
	:Enemy(gameData, name, animation, static_cast<unsigned int>(movementSpeed), hp, maxHp, posAndSize, mass, damage)
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
	handlePlayerHit();

	if(mHP <= 0) {
		auto enemyContainer = dynamic_cast<EnemyContainer*>(mParent);
		enemyContainer->addEnemyToDie(this);
	}

	move(delta);
}

void Zombie::handlePlayerHit()
{
	auto& gameScene = mGameData->getSceneMachine().getScene();
	auto& root = gameScene.getRoot();
	Character& player = dynamic_cast<Character&>(root.getChild("player"));

	if (Math::areTheyOverlapping(getSprite().getGlobalBounds(), player.getSprite().getGlobalBounds()))
		player.takeDamage(damage);
}

void Zombie::move(sf::Time delta)
{
	setPosition(mCollisionBody.getPosition());

	if(mMovementPath.empty()) {
		mMovementPath = mGameData->getAIManager().getZombiePath(mPosition);
		mTimeFromStartingThisMove.restart();
	}

	if(mTimeFromStartingThisMove.getElapsedTime().asSeconds() > mTimeInSecondsToMoveToAnotherTile) {
		mTimeFromStartingThisMove.restart();
		Direction currentDirection = mMovementPath.front();
		mMovementPath.pop_front();
		mCurrentDirectionVector = toDirectionVector(currentDirection);
	}

	mCollisionBody.move(movementSpeed * delta.asSeconds() * mCurrentDirectionVector);
}

sf::Vector2f Zombie::toDirectionVector(Direction direction)
{
	switch(direction)
	{
	case ph::Direction::east:
		return sf::Vector2f(1.f, 0.f);
	case ph::Direction::west:
		return sf::Vector2f(-1.f, 0.f);
	case ph::Direction::north:
		return sf::Vector2f(0.f, -1.f);
	case ph::Direction::south:
		return sf::Vector2f(0.f, 1.f);
	case ph::Direction::none:
		return sf::Vector2f();
	default:
		PH_UNEXPECTED_SITUATION("Not all directions were handled in switch");
	}
}

}