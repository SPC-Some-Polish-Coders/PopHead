#include "zombie.hpp"
#include "gameData.hpp"
#include "Resources/collisionRectData.hpp"
#include "Resources/spriteSheetData.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "GameObjects/GameObjectContainers/gameObjectLayers.hpp"
#include "Utilities/random.hpp"
#include "GameObjects/DrawableGameObjects/Items/bulletItem.hpp"
#include "GameObjects/NonDrawableGameObjects/equipment.hpp"

namespace ph {

namespace
{
	const std::string name = "zombie";
	constexpr float movementSpeed = 50.f;
	constexpr unsigned damage = 20;
	constexpr unsigned maxHp = 100;
	constexpr int hp = 100;
	const sf::FloatRect posAndSize(
		0,
		0,
		CollisionRectData::HUMAN_WIDTH,
		CollisionRectData::HUMAN_HEIGHT
	);
	constexpr float mass = 40;

	const Animation animation{
		std::array<std::string, 13>{
			"down", "right", "left", "rightUp", "leftUp", "up",
			"fightDown", "fightRight", "fightLeft", "fightRightUp", "fightLeftUp", "fightUp",
			"dead"
		},
		std::array<sf::IntRect, 13>{
			sf::IntRect(0, 0 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 1 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 2 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 3 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 4 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 5 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 6 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 7 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 8 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 9 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 10 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 11 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 12 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
		},
		{
			4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1
		},
		sf::seconds(0.10f)
	};
}

Zombie::Zombie(GameData* gameData)
	:Enemy(gameData, name, animation, static_cast<unsigned int>(movementSpeed), hp, maxHp, posAndSize, mass, damage)
	,mIsSlownDown(false)
{
	mSprite.setTexture(gameData->getTextures().get("textures/characters/zombieFullAnimation.png"));
	mAnimation.animate(mSprite);

	//temporary random generate until we develop this system
	int numberOfBullets = Random::generateNumber(0, 1);
	for(int i = 0; i < numberOfBullets; ++i)
		dynamic_cast<Equipment*>(getChild("Equipment"))->putItem(std::make_unique<BulletItem>(mGameData));
}

void Zombie::updateCurrent(sf::Time delta)
{
	if(isDead()) {
		mAnimation.changeState("dead");
		mAnimation.animate(mSprite);
		mGameData->getPhysicsEngine().removeKinematicBody(mCollisionBody);
		auto standingGameObjectsLayer = dynamic_cast<StandingGameObjectsLayer*>(mParent);
		standingGameObjectsLayer->addCharacterToDie(this);
		return;
	}

	if(timeFromLastGrowl.getElapsedTime().asSeconds() > 2) {
		int randomNumber = Random::generateNumber(1, 4);
		auto& soundPlayer = mGameData->getSoundPlayer();
		switch(randomNumber)
		{
			case 1: soundPlayer.playSpatialSound("sounds/zombieGrowl1.ogg", getPosition()); break;
			case 2: soundPlayer.playSpatialSound("sounds/zombieGrowl2.ogg", getPosition()); break;
			case 3: soundPlayer.playSpatialSound("sounds/zombieGrowl3.ogg", getPosition()); break;
			case 4: soundPlayer.playSpatialSound("sounds/zombieGrowl4.ogg", getPosition()); break;
			default:
				PH_UNEXPECTED_SITUATION("Random sound choosing in zombie failed!");
		}
		timeFromLastGrowl.restart();
	}

	setPosition(mCollisionBody.getFixedPosition());
	if(mGameData->getAIManager().isPlayerOnScene())
		handlePlayerHit();
	move(delta);
	updateAnimation(delta);

	mIsSlownDown = false;
}

void Zombie::handlePlayerHit()
{
	auto* playerGameObject = mRoot->getChild("LAYER_standingObjects")->getChild("player");
	if(playerGameObject == nullptr)
		return;
	auto* player = dynamic_cast<Character*>(playerGameObject);
	const sf::Vector2f zombieWorldPosition = getWorldPosition();
	FloatRect zombieDamageArea(zombieWorldPosition.x - 0.3f, zombieWorldPosition.y - 0.3f, 20.6f, 20.6f);
	if(zombieDamageArea.doPositiveRectsIntersect(player->getGlobalBounds()))
		player->takeDamage(damage);
}

void Zombie::move(sf::Time delta)
{
	setPosition(mCollisionBody.getFixedPosition());

	if(mMovementPath.mPath.empty()) {
		mMovementPath = mGameData->getAIManager().getZombiePath(mCollisionBody.getPosition());
		mTimeFromStartingThisMove.restart();
	}

	if(mTimeFromStartingThisMove.getElapsedTime().asSeconds() > mTimeInSecondsToMoveToAnotherTile) {
		mTimeFromStartingThisMove.restart();
		Direction currentDirection = mMovementPath.mPath.front();
		mMovementPath.mPath.pop_front();
		mCurrentDirectionVector = toDirectionVector(currentDirection);
	}

	if (mMovementPath.mIsAttackingPath)
		mIsInAttackingMode = true;

	const float currentMovementSpeed = mIsSlownDown ? mMovementSpeed / 1.6f : mMovementSpeed;
	Character::move(currentMovementSpeed * delta.asSeconds() * mCurrentDirectionVector);
	//mCollisionBody.move(currentMovementSpeed * delta.asSeconds() * mCurrentDirectionVector);
}

sf::Vector2f Zombie::toDirectionVector(Direction direction)
{
	static const float sq = static_cast<float>(std::sqrt(2)) / 2.f;
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
	case ph::Direction::north_east:
		return sf::Vector2f(sq, -sq);
	case ph::Direction::north_west:
		return sf::Vector2f(-sq, -sq);
	case ph::Direction::south_east:
		return sf::Vector2f(sq, sq);
	case ph::Direction::south_west:
		return sf::Vector2f(-sq, sq);
	case ph::Direction::none:
		return sf::Vector2f();
	default:
		PH_UNEXPECTED_SITUATION("Not all directions were handled in switch");
	}
}

void Zombie::updateAnimation(sf::Time delta)
{
	if(mIsSlownDown)
		mAnimation.setDelay(sf::seconds(0.24f));
	else
		mAnimation.setDelay(sf::seconds(0.12f));

	if(mGameData->getAIManager().shouldZombiePlayAttackAnimation(mCollisionBody.getPosition())) {
		if(mCurrentDirectionVector == sf::Vector2f(1.f, 0.f))
			setAnimationState("fightRight");
		else if(mCurrentDirectionVector == sf::Vector2f(-1.f, 0.f))
			setAnimationState("fightLeft");
		else if(mCurrentDirectionVector == sf::Vector2f(0.f, -1.f))
			setAnimationState("fightUp");
		else if(mCurrentDirectionVector == sf::Vector2f(0.f, 1.f))
			setAnimationState("fightDown");
	}
	else {
		if(mCurrentDirectionVector == sf::Vector2f(1.f, 0.f))
			setAnimationState("right");
		else if(mCurrentDirectionVector == sf::Vector2f(-1.f, 0.f))
			setAnimationState("left");
		else if(mCurrentDirectionVector == sf::Vector2f(0.f, -1.f))
			setAnimationState("up");
		else if(mCurrentDirectionVector == sf::Vector2f(0.f, 1.f))
			setAnimationState("down");
	}

	mAnimation.animate(mSprite, delta);
}

void Zombie::setAnimationState(const std::string& stateName)
{
	const std::string name = mAnimation.getCurrentStateName();
	if(name != stateName)
		mAnimation.changeState(stateName);
}

}
