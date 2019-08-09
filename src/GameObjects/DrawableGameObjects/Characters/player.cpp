#include "player.hpp"
#include "gameData.hpp"
#include "Resources/spriteSheetData.hpp"
#include "Resources/collisionRectData.hpp"
#include "Utilities/animation.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "GameObjects/DrawableGameObjects/gun.hpp"
#include "GameObjects/DrawableGameObjects/melee.hpp"
#include <array>

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
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1
		},
		sf::seconds(0.10f)
	);
}

Player::Player(GameData* gameData)
	:Character(gameData, name, animation, movementSpeed, HP, maxHP, posAndSize, mass)
	,mMotion()
	,mLastMotion()
	,mIsShooting(false) 
	,mIsAttacking(false)
{
	mAnimation.animate(mSprite);
	addChild(std::make_unique<Gun>(mGameData, 5.f));
	addChild(std::make_unique<MeleeWeapon>(mGameData, 25.f, 25.f, 60.f));
}

void Player::input()
{
	movementInput();
	gunInput();
	meleeWeaponInput();
}

void Player::movementInput()
{
	if (mGameData->getInput().getAction().isActionPressed("movingLeft"))
		mMotion.isMovingLeft = true;
	if (mGameData->getInput().getAction().isActionPressed("movingRight"))
		mMotion.isMovingRight = true;
	if (mGameData->getInput().getAction().isActionPressed("movingUp"))
		mMotion.isMovingUp = true;
	if (mGameData->getInput().getAction().isActionPressed("movingDown"))
		mMotion.isMovingDown = true;
}

void Player::gunInput()
{
	if(mGameData->getInput().getAction().isActionJustPressed("attack"))
		mIsShooting = true;
}

void Player::meleeWeaponInput()
{
	if(mGameData->getInput().getAction().isActionJustPressed("meleeAttack"))
		mIsAttacking = true;
}

void Player::update(sf::Time delta)
{
	updateMovement(delta);
	updateAnimation(delta);
	mMotion.clear();
	shootingUpdate(delta);
	meleeAttackUpdate(delta);
	cameraMovement(delta);
	updateListenerPosition();

	for(auto& child : mChildren)
		child->update(delta);
}

void Player::updateMovement(const sf::Time delta)
{
	sf::Vector2f velocity;

	if (mMotion.isMoving() && !mCollisionBody.isBeingPushed())
	{
		mLastMotion = mMotion;
		if (mMotion.isMovingLeft)
			velocity.x -= mMovementSpeed * delta.asSeconds();
		if (mMotion.isMovingRight)
			velocity.x += mMovementSpeed * delta.asSeconds();
		if (mMotion.isMovingUp)
			velocity.y -= mMovementSpeed * delta.asSeconds();
		if (mMotion.isMovingDown)
			velocity.y += mMovementSpeed * delta.asSeconds();

		if (mMotion.isMovingDiagonally()) {
			velocity.x *= std::sqrt(2.f) / 2.f;
			velocity.y *= std::sqrt(2.f) / 2.f;
		}

		mCollisionBody.move(velocity);
	}
	setPosition(mCollisionBody.getPosition());
	mGameData->getAIManager().setPlayerPosition(mPosition);
}

void Player::updateAnimation(const sf::Time delta)
{
	if(mTimeFromLastMeleeAtack.getElapsedTime().asSeconds() < 0.15f) {
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

	mAnimation.animate(mSprite, delta);
}

void Player::setAnimationState(const std::string& stateName)
{
	const std::string name = mAnimation.getCurrentStateName();
	if (name != stateName) {
		mAnimation.changeState(stateName);
		mAnimation.animate(mSprite);
	}
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

void Player::shootingUpdate(const sf::Time delta)
{
	if(mIsShooting) {
		sf::Vector2f shotDirection = attackDirection();
		auto& gun = dynamic_cast<Gun&>(getChild("gun"));
		gun.shoot(shotDirection);
		mIsShooting = false;
	}
}

void Player::meleeAttackUpdate(const sf::Time delta)
{
	if (mIsAttacking) {
		mTimeFromLastMeleeAtack.restart();
		sf::Vector2f meleeAttackDirection = attackDirection();
		auto& meleeWeapon = dynamic_cast<MeleeWeapon&>(getChild("sword"));
		meleeWeapon.attack(meleeAttackDirection);
		mIsAttacking = false;
	}
}

sf::Vector2f Player::attackDirection()
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

void Player::cameraMovement(sf::Time delta) const
{
	constexpr float cameraMotionSpeed = 4.f;
	const sf::FloatRect characterBounds = mSprite.getGlobalBounds();
	mGameData->getRenderer().moveCamera(Math::getCenter(characterBounds), cameraMotionSpeed * delta.asSeconds());
}

void Player::updateListenerPosition() const
{
	mGameData->getSoundPlayer().setListenerPosition(getPosition());
}

}