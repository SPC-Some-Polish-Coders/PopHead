#include "player.hpp"

#include "Base/gameData.hpp"
#include <array>

#include <iostream>

using PopHead::World::Entity::Player;

Player::Player(PopHead::Base::GameData* gameData)
:Character(gameData, "player", 150)
{
    mSprite.setPosition(400, 400);

    const std::array<std::string, 4> statesNames
	{
		"down",
		"left",
		"right",
		"up",
	};
	const int frameWidth = 48;
	const int frameHeight = 48;
	const unsigned framesCount = 3;
	for (unsigned top = 0; top < statesNames.size(); ++top){
		const sf::IntRect frame(0, top * frameHeight, frameWidth, frameHeight);
		mAnimation.addState(statesNames[top], frame, framesCount);
	}
	mAnimation.setDelay(sf::seconds(0.15f));
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
		mAnimation.changeState(stateName);
}

/*void talkTo(NPC&)
{

}*/

//auto getPerks() const -> const PerkManager& { return mPerks; }
