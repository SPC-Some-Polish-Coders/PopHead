#include "player.hpp"

#include "Base/gameData.hpp"

#include "Resources/spriteSheetData.hpp"

#include <array>

using PopHead::World::Entity::Player;
using namespace PopHead::Resources;

Player::Player(PopHead::Base::GameData* gameData)
	:
	Character(gameData, "player",
		Animation{
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
		}, 150, 100, 100, sf::FloatRect(0, 0, SpriteSheetData::PLAYER_WIDTH, SpriteSheetData::PLAYER_HEIGHT), 50)
{
    mSprite.setPosition(400, 400);
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

        mAnimation.animate(mSprite, delta);
    }

    mSprite.move(velocity);

    mMotion.clear();
}

void Player::updateAnimation(const std::string& stateName)
{
    const std::string name = mAnimation.getCurrentStateName();
	if (name != stateName)
	{
		mAnimation.changeState(stateName);
		mAnimation.animate(mSprite);
	}
}

/*void talkTo(NPC&)
{

}*/

//auto getPerks() const -> const PerkManager& { return mPerks; }
