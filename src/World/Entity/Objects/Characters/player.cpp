#include "player.hpp"

#include "Base/gameData.hpp"
#include <array>

#include <iostream>

using PopHead::World::Entity::Player;

Player::Player(PopHead::Base::GameData* gameData)
	:
	Character(gameData, "player",
		Animation{
			std::array<std::string, 4>{"down", "left", "right", "up"},
			{sf::IntRect(0,0,48,48), sf::IntRect(0,48,48,48), sf::IntRect(0,96,48,48), sf::IntRect(0,144,48,48)},
			{3,3,3,3},
			sf::seconds(0.15f)
		}, 150)
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

    #if 0
    if(INPUT.is_action_pressed("movingLeft")){
        mMotion.isMovingLeft = true;
    }
    if(INPUT.is_action_pressed("movingRight")){
        mMotion.isMovingRight = true;
    }
    if(INPUT.is_action_pressed("movingUp")){
        mMotion.isMovingUp = true;
    }
    if(INPUT.is_action_pressed("movingDown")){
        mMotion.isMovingDown = true;
    }
    #endif // 0
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
