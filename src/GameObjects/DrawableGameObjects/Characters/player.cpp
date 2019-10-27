#include "player.hpp"
#include "gameData.hpp"
#include "Events/actionEventManager.hpp"
#include "Resources/spriteSheetData.hpp"
#include "Resources/collisionRectData.hpp"
#include "Utilities/animation.hpp"
#include "Utilities/rect.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "GameObjects/DrawableGameObjects/gun.hpp"
#include "GameObjects/DrawableGameObjects/melee.hpp"
#include "GameObjects/DrawableGameObjects/Items/bulletItem.hpp"
#include "GameObjects/NonDrawableGameObjects/playerEquipment.hpp"
#include "GameObjects/GameObjectContainers/gameObjectLayers.hpp"
#include "GameObjects/NonDrawableGameObjects/arcadeManager.hpp"
#include <array>
#include <exception>

namespace ph {

namespace
{
	const std::string name = "player";
	const unsigned int movementSpeed = 55;
	const unsigned int HP = 100;
	const unsigned int maxHP = 100;
	const sf::FloatRect posAndSize(
		50,
		50,
		CollisionRectData::HUMAN_WIDTH,
		CollisionRectData::HUMAN_HEIGHT
	);
	float mass = 25;

	Animation animation(
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
		{SpriteSheetData::HUMAN_TEXTURE_WIDTH, SpriteSheetData::HUMAN_TEXTURE_HEIGHT},
		sf::seconds(0.12f)
	);
}

const sf::Time Player::meleeAttackInterval = sf::seconds(0.5f);

Player::Player(GameData* gameData)
	:Character(gameData, name, gameData->getTextures().get("textures/characters/playerFullAnimation.png"), 
		animation, movementSpeed, HP, maxHP, posAndSize, mass, false)
	,mMotion()
	,mLastMotion()
	,mIsSlownDown(false)
	,mPickRadius(10.f)
	,mIsDead(false)
{
	mAnimation.animate();
	addChild(std::make_unique<Gun>(mGameData->getSoundPlayer(), mGameData->getTextures().get("textures/others/pistol.png"), 5.f));

	const float meleeWeaponDamage = 35.f;
	const float range = 27.f;
	const float rotationRange = 100.f;
	addChild(std::make_unique<MeleeWeapon>(mGameData, meleeWeaponDamage, range, rotationRange));

	setNumOfBullets(60u);
}

void Player::handleEventOnCurrent(const ph::Event& phEvent)
{
	bool isGamePaused = mGameData->getSceneManager().getScene().getPause();
	if(auto* actionEvent = std::get_if<ActionEvent>(&phEvent))
	{
		if(actionEvent->mType == ActionEvent::Pressed)
		{
			if(!isGamePaused){
				if(actionEvent->mAction == "gunAttack" && mNumberOfOwnedBullets > 0) {
					dynamic_cast<PlayerEquipment*>(getChild("Equipment"))->destroyItem("Bullet");
					auto* gun = dynamic_cast<Gun*>(getChild("gun"));
					gun->shoot();
				}
				else if(actionEvent->mAction == "meleeAtack" && mTimeFromLastMeleeAttack.getElapsedTime() >= meleeAttackInterval) {
					mTimeFromLastMeleeAttack.restart();
					auto* meleeWeapon = dynamic_cast<MeleeWeapon*>(getChild("sword"));
					sf::Vector2f meleeAttackDirection = getCurrentPlayerDirection();
					meleeWeapon->attack(meleeAttackDirection, getPlayerRotation());
				}
			}

			if(actionEvent->mAction == "pauseScreen")
			{
				if(isGamePaused) {
					mGameData->getGui().hideInterface("pauseScreen");
					mGameData->getSceneManager().getScene().setPause(false);
				}
				else {
					mGameData->getGui().showInterface("pauseScreen");
					mGameData->getSceneManager().getScene().setPause(true);
				}
			}
		}
	}
}

void Player::updateCurrent(sf::Time delta)
{
	updateCounters();

	if(Character::isDead()) {
		die();
		return;
	}
		
	updateMovement(delta);
	updateAnimation(delta);
	mMotion.clear();
	shootingUpdate(delta);
	cameraMovement(delta);
	updateListenerPosition();

	mNumberOfOwnedBullets = dynamic_cast<PlayerEquipment*>(getChild("Equipment"))->getItemQuantity("Bullet");

	mIsSlownDown = false;

	if(!ArcadeManager::isActive()){
		bool isAttacked = mIsInAttackingMode;
		mIsInAttackingMode = false;
		if (isAttacked == mIsAttacked)
			return;
		mIsAttacked = isAttacked;

		if (mIsAttacked)
			mGameData->getMusicPlayer().playFromMusicState("fighting");
		else
			mGameData->getMusicPlayer().playFromMusicState("exploration");
	}
}

unsigned Player::getNumOfBullets() const
{
	return mNumberOfOwnedBullets;
}

void Player::setNumOfBullets(unsigned num)
{
	mNumberOfOwnedBullets = num;
	
	removeChild("Equipment");
	addChild(std::make_unique<PlayerEquipment>());
	auto* equipment = dynamic_cast<PlayerEquipment*>(getChild("Equipment"));
	equipment->init();
	for (unsigned i = 0; i < mNumberOfOwnedBullets; ++i)
		equipment->putItem(std::make_unique<BulletItem>(mGameData));
}

void Player::die()
{
	setAnimationState("dead");
	auto standingObjects = dynamic_cast<StandingGameObjectsLayer*>(mParent);
	standingObjects->addCharacterToDie(this);
	mGameData->getGui().showInterface("gameOverScreen");
	mGameData->getAIManager().setIsPlayerOnScene(false);
	mIsDead = true;
}

void Player::updateCounters() const
{
	auto gameplayCounters = mGameData->getGui().getInterface("gameplayCounters");
	auto canvas = gameplayCounters->getWidget("canvas");
	try {
		auto vitalityCounter = dynamic_cast<TextWidget*>(canvas->getWidget("vitalityCounter"));
		vitalityCounter->setString(std::to_string(mHp));

		auto bulletCounter = dynamic_cast<TextWidget*>(canvas->getWidget("bulletCounter"));
		bulletCounter->setString(std::to_string(mNumberOfOwnedBullets));
	}
	catch(const std::exception& e) {
		PH_LOG_ERROR("Setting values to gameplay counters failed! (" + std::string(e.what()) + ")");
	}
}

void Player::updateMovement(const sf::Time delta)
{
	if(ActionEventManager::isActionPressed("movingLeft"))
		mMotion.isMovingLeft = true;
	if(ActionEventManager::isActionPressed("movingRight"))
		mMotion.isMovingRight = true;
	if(ActionEventManager::isActionPressed("movingUp"))
		mMotion.isMovingUp = true;
	if(ActionEventManager::isActionPressed("movingDown"))
		mMotion.isMovingDown = true;

	sf::Vector2f velocity;
	const float currentMovementSpeed = mIsSlownDown ? mMovementSpeed / 1.8f : mMovementSpeed;

	if (mMotion.isMoving() && !mCollisionBody.isBeingPushed())
	{
		mLastMotion = mMotion;
		if (mMotion.isMovingLeft)
			velocity.x -= currentMovementSpeed * delta.asSeconds();
		if (mMotion.isMovingRight)
			velocity.x += currentMovementSpeed * delta.asSeconds();
		if (mMotion.isMovingUp)
			velocity.y -= currentMovementSpeed * delta.asSeconds();
		if (mMotion.isMovingDown)
			velocity.y += currentMovementSpeed * delta.asSeconds();

		if (mMotion.isMovingDiagonally()) {
			velocity.x *= std::sqrt(2.f) / 2.f;
			velocity.y *= std::sqrt(2.f) / 2.f;
		}

		move(velocity);
	}

	setPosition(mCollisionBody.getFixedPosition());

	FloatRect collisionRect = mCollisionBody.getRect();
	mGameData->getAIManager().setPlayerPosition(collisionRect.getCenter());
}

void Player::updateAnimation(const sf::Time delta)
{
	if(mIsSlownDown)
		mAnimation.setDelay(sf::seconds(0.24f));
	else
		mAnimation.setDelay(sf::seconds(0.12f));

	if(mTimeFromLastMeleeAttack.getElapsedTime().asSeconds() < 0.15f) {
		if(mLastMotion.isMovingLeft && mLastMotion.isMovingUp)
			setAnimationState("fightLeftUp");
		else if(mLastMotion.isMovingRight && mLastMotion.isMovingUp)
			setAnimationState("fightRightUp");
		else if(mLastMotion.isMovingLeft)
			setAnimationState("fightLeft");
		else if(mLastMotion.isMovingRight)
			setAnimationState("fightRight");
		else if(mLastMotion.isMovingUp)
			setAnimationState("fightUp");
		else if(mLastMotion.isMovingDown)
			setAnimationState("fightDown");
	}
	else {
		if(mLastMotion.isMovingLeft && mLastMotion.isMovingUp)
			setAnimationState("leftUp");
		else if(mLastMotion.isMovingRight && mLastMotion.isMovingUp)
			setAnimationState("rightUp");
		else if(mLastMotion.isMovingLeft)
			setAnimationState("left");
		else if(mLastMotion.isMovingRight)
			setAnimationState("right");
		else if(mLastMotion.isMovingUp)
			setAnimationState("up");
		else if(mLastMotion.isMovingDown)
			setAnimationState("down");
	}
	
	if(!mMotion.isMoving()) {
		mAnimation.goToFrontFrame();
		return;
	}

	mAnimation.animate(delta);
}

void Player::setAnimationState(const std::string& stateName)
{
	const std::string name = mAnimation.getCurrentStateName();
	if (name != stateName) {
		mAnimation.changeState(stateName);
		mAnimation.animate();
	}
}

void Player::shootingUpdate(const sf::Time delta)
{
	auto* gun = dynamic_cast<Gun*>(getChild("gun"));
	gun->setCurrentPlayerDirection(getCurrentPlayerDirection());
}

PlayerMotion::PlayerMotion()
{
	clear();
}

bool PlayerMotion::isMoving()
{
	return isMovingLeft || isMovingRight || isMovingUp || isMovingDown;
}

bool PlayerMotion::isMovingDiagonally()
{
	return (isMovingLeft || isMovingRight) && (isMovingUp || isMovingDown);
}

void PlayerMotion::clear()
{
	isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
}

sf::Vector2f Player::getCurrentPlayerDirection() const
{
	if (mLastMotion.isMovingRight && mLastMotion.isMovingUp)
		return  { 0.7f, -0.7f };
	else if (mLastMotion.isMovingLeft && mLastMotion.isMovingUp)
		return  { -0.7f, -0.7f };
	else if (mLastMotion.isMovingRight && mLastMotion.isMovingDown)
		return  { 0.7f, 0.7f };
	else if (mLastMotion.isMovingLeft && mLastMotion.isMovingDown)
		return  { -0.7f, 0.7f };
	else if (mLastMotion.isMovingRight)
		return  { 1.f, 0.f };
	else if (mLastMotion.isMovingLeft)
		return  { -1.f, 0.f };
	else if (mLastMotion.isMovingUp)
		return  { 0.f, -1.f };
	else if (mLastMotion.isMovingDown)
		return  { 0.f, 1.f };
	else
		return  {0.f, 1.f};
}

float Player::getPlayerRotation() const
{
	if (mLastMotion.isMovingRight && mLastMotion.isMovingDown)
		return 45.f;
	else if (mLastMotion.isMovingLeft && mLastMotion.isMovingDown)
		return 135.f;
	else if (mLastMotion.isMovingLeft && mLastMotion.isMovingUp)
		return 225.f;
	else if (mLastMotion.isMovingRight && mLastMotion.isMovingUp)
		return 315.f;
	else if (mLastMotion.isMovingRight)
		return 0.f;
	else if (mLastMotion.isMovingDown)
		return 90.f;
	else if (mLastMotion.isMovingLeft)
		return 180.f;
	else if (mLastMotion.isMovingUp)
		return 270.f;
	
	PH_LOG_WARNING("Unsupported player rotation");
	return 90.f;
}

void Player::cameraMovement(sf::Time delta) const
{
	constexpr float cameraMotionSpeed = 4.f;
	const FloatRect characterBounds = GameObject::getGlobalBounds();
	mPlayerCamera.setCenterSmoothly(characterBounds.getCenter(), cameraMotionSpeed * delta.asSeconds());
}

void Player::updateListenerPosition() const
{
	mGameData->getSoundPlayer().setListenerPosition(getPosition());
}

}
