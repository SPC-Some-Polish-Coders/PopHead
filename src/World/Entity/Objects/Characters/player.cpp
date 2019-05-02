#include "player.hpp"

#include "Base/gameData.hpp"
#include "Resources/spriteSheetData.hpp"
#include "World/Animation/animation.hpp"

#include <array>

using PopHead::World::Entity::Player;

namespace
{
	using namespace PopHead::Resources;

	std::string name = "player";
	unsigned int movementSpeed = 150;
	unsigned int HP = 100;
	unsigned int maxHP = 100;
	sf::FloatRect posAndSize = sf::FloatRect(0, 0, SpriteSheetData::PLAYER_WIDTH, SpriteSheetData::PLAYER_HEIGHT);
	float mass = 50;

	PopHead::World::Animation animation{
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

Player::Player(PopHead::Base::GameData* gameData)
	:Character(gameData, name, std::move(animation), movementSpeed, HP, maxHP, posAndSize, mass)
{
	mAnimation.animate(mSprite);
}

void Player::input()
{
    if(INPUT_isActionPressed("movingLeft")){
        mMotion.isMovingLeft = true;
    }
    if(INPUT_isActionPressed("movingRight")){
        mMotion.isMovingRight = true;
    }
    if(INPUT_isActionPressed("movingUp")){
        mMotion.isMovingUp = true;
    }
    if(INPUT_isActionPressed("movingDown")){
        mMotion.isMovingDown = true;
    }
}

void Player::update(sf::Time delta)
{
    sf::Vector2f velocity;

    if(mMotion.isMoving())
    {
        if(mMotion.isMovingLeft){
            velocity.x -= mMovementSpeed * delta.asSeconds();
            updateAnimation("left");
        }
        if(mMotion.isMovingRight){
            velocity.x += mMovementSpeed * delta.asSeconds();
            updateAnimation("right");
        }
        if(mMotion.isMovingUp){
            velocity.y -= mMovementSpeed * delta.asSeconds();
            updateAnimation("up");
        }
        if(mMotion.isMovingDown){
            velocity.y += mMovementSpeed * delta.asSeconds();
            updateAnimation("down");
        }
		mCollisionBody.move(velocity);
        mAnimation.animate(mSprite, delta);
    }
    mMotion.clear();
}

void Player::updateAnimation(const std::string& stateName)
{
    const std::string name = mAnimation.getCurrentStateName();
	if (name != stateName){
		mAnimation.changeState(stateName);
		mAnimation.animate(mSprite);
	}
}

/*void talkTo(NPC&)
{

}*/

//auto getPerks() const -> const PerkManager& { return mPerks; }
