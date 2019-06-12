#include "player.hpp"

#include "Base/gameData.hpp"
#include "Resources/spriteSheetData.hpp"
#include "Resources/collisionRectData.hpp"
#include "World/Animation/animation.hpp"
#include <array>

namespace
{
	std::string name = "player";
	unsigned int movementSpeed = 150;
	unsigned int HP = 100;
	unsigned int maxHP = 100;
	sf::FloatRect posAndSize(
		0,
		0,
		ph::CollisionRectData::PLAYER_WIDTH,
		ph::CollisionRectData::PLAYER_HEIGHT
	);
	float mass = 50;

	ph::Animation animation{
		std::array<std::string, 4>{"down", "left", "right", "up"},
		{
			sf::IntRect(0, 0 * ph::SpriteSheetData::PLAYER_HEIGHT, ph::SpriteSheetData::PLAYER_WIDTH, ph::SpriteSheetData::PLAYER_HEIGHT),
			sf::IntRect(0, 1 * ph::SpriteSheetData::PLAYER_HEIGHT, ph::SpriteSheetData::PLAYER_WIDTH, ph::SpriteSheetData::PLAYER_HEIGHT),
			sf::IntRect(0, 2 * ph::SpriteSheetData::PLAYER_HEIGHT, ph::SpriteSheetData::PLAYER_WIDTH, ph::SpriteSheetData::PLAYER_HEIGHT),
			sf::IntRect(0, 3 * ph::SpriteSheetData::PLAYER_HEIGHT, ph::SpriteSheetData::PLAYER_WIDTH, ph::SpriteSheetData::PLAYER_HEIGHT)
		},
		{
			ph::SpriteSheetData::PLAYER_FRAMES_COUNT,
			ph::SpriteSheetData::PLAYER_FRAMES_COUNT,
			ph::SpriteSheetData::PLAYER_FRAMES_COUNT,
			ph::SpriteSheetData::PLAYER_FRAMES_COUNT
		},
		sf::seconds(0.15f)
	};
}

ph::Player::Player(GameData* gameData)
	:Character(gameData, name, std::move(animation), movementSpeed, HP, maxHP, posAndSize, mass)
{
	mAnimation.animate(mSprite);
}

void ph::Player::input()
{
	if (mGameData->getInput().getAction().isActionPressed("movingLeft")) {
		mMotion.isMovingLeft = true;
	}
	if (mGameData->getInput().getAction().isActionPressed("movingRight")) {
		mMotion.isMovingRight = true;
	}
	if (mGameData->getInput().getAction().isActionPressed("movingUp")) {
		mMotion.isMovingUp = true;
	}
	if (mGameData->getInput().getAction().isActionPressed("movingDown")) {
		mMotion.isMovingDown = true;
	}
}

void ph::Player::update(sf::Time delta)
{
	sf::Vector2f velocity;

	if (mMotion.isMoving() && !mCollisionBody.isBeingPushed())
	{
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
}

void ph::Player::updateAnimation(const std::string& stateName)
{
	const std::string name = mAnimation.getCurrentStateName();
	if (name != stateName) {
		mAnimation.changeState(stateName);
		mAnimation.animate(mSprite);
	}
}

/*void talkTo(Npc&)
{

}*/

//auto getPerks() const -> const PerkManager& { return mPerks; }
