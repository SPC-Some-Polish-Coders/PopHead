#include "player.hpp"
#include "gameData.hpp"
#include "Resources/spriteSheetData.hpp"
#include "Resources/collisionRectData.hpp"
#include "Utilities/animation.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "GameObjects/DrawableGameObjects/gun.hpp"
#include <array>

namespace ph {

namespace
{
	std::string name = "player";
	unsigned int movementSpeed = 75;
	unsigned int HP = 100;
	unsigned int maxHP = 100;
	sf::FloatRect posAndSize(
		0,
		0,
		CollisionRectData::PLAYER_WIDTH,
		CollisionRectData::PLAYER_HEIGHT
	);
	float mass = 25;

	Animation animation{
		std::array<std::string, 4>{"down", "left", "right", "up"},
		{
			sf::IntRect(0, 0 * SpriteSheetData::PLAYER_HEIGHT, SpriteSheetData::PLAYER_WIDTH, SpriteSheetData::PLAYER_HEIGHT),
			sf::IntRect(0, 1 * SpriteSheetData::PLAYER_HEIGHT, SpriteSheetData::PLAYER_WIDTH, SpriteSheetData::PLAYER_HEIGHT),
			sf::IntRect(0, 2 * SpriteSheetData::PLAYER_HEIGHT, SpriteSheetData::PLAYER_WIDTH, SpriteSheetData::PLAYER_HEIGHT),
			sf::IntRect(0, 3 * SpriteSheetData::PLAYER_HEIGHT, SpriteSheetData::PLAYER_WIDTH, SpriteSheetData::PLAYER_HEIGHT)
		},
		{
			SpriteSheetData::PLAYER_FRAMES_COUNT,
			SpriteSheetData::PLAYER_FRAMES_COUNT,
			SpriteSheetData::PLAYER_FRAMES_COUNT,
			SpriteSheetData::PLAYER_FRAMES_COUNT
		},
		sf::seconds(0.15f)
	};
}

Player::Player(GameData* gameData)
	:Character(gameData, name, animation, movementSpeed, HP, maxHP, posAndSize, mass)
	,mIsShooting(false)
{
	mAnimation.animate(mSprite);
	addChild(std::make_unique<Gun>(mGameData, 5));
}

void Player::input()
{
	movementInput();
	gunInput();

	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::F7))
		mGameData->getSceneMachine().replaceScene("scenes/smallScene.xml");
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

void Player::update(sf::Time delta)
{
	updateMovement(delta);
	shootingUpdate(delta);
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
		if (mMotion.isMovingLeft) {
			velocity.x -= mMovementSpeed * delta.asSeconds();
			updateAnimation("left");
		}
		if (mMotion.isMovingRight) {
			velocity.x += mMovementSpeed * delta.asSeconds();
			updateAnimation("right");
		}
		if (mMotion.isMovingUp) {
			velocity.y -= mMovementSpeed * delta.asSeconds();
			updateAnimation("up");
		}
		if (mMotion.isMovingDown) {
			velocity.y += mMovementSpeed * delta.asSeconds();
			updateAnimation("down");
		}

		if (mMotion.isMovingDiagonally()) {
			velocity.x *= std::sqrt(2.f) / 2.f;
			velocity.y *= std::sqrt(2.f) / 2.f;
		}

		mCollisionBody.move(velocity);
		mAnimation.animate(mSprite, delta);
	}
	mMotion.clear();
	setPosition(mCollisionBody.getPosition());
	mGameData->getAIManager().setPlayerPosition(mPosition);
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
		sf::Vector2f shotDirection;
		if(mLastMotion.isMovingRight && mLastMotion.isMovingUp)
			shotDirection = {0.7f, -0.7f};
		else if(mLastMotion.isMovingLeft && mLastMotion.isMovingUp)
			shotDirection = {-0.7f, -0.7f};
		else if(mLastMotion.isMovingRight && mLastMotion.isMovingDown)
			shotDirection = {0.7f, 0.7f};
		else if(mLastMotion.isMovingLeft && mLastMotion.isMovingDown)
			shotDirection = {-0.7f, 0.7f};
		else if(mLastMotion.isMovingRight)
			shotDirection = {1.f, 0.f};
		else if(mLastMotion.isMovingLeft)
			shotDirection = {-1.f, 0.f};
		else if(mLastMotion.isMovingUp)
			shotDirection = {0.f, -1.f};
		else if(mLastMotion.isMovingDown)
			shotDirection = {0.f, 1.f};

		auto& gun = dynamic_cast<Gun&>(getChild("gun"));
		gun.shoot(shotDirection);

		mIsShooting = false;
	}
}

void Player::updateAnimation(const std::string& stateName)
{
	const std::string name = mAnimation.getCurrentStateName();
	if (name != stateName) {
		mAnimation.changeState(stateName);
		mAnimation.animate(mSprite);
	}
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