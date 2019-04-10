#include "player.hpp"

#include "Base/gameData.hpp"

#include <iostream>

using PopHead::World::Entity::Player;

Player::Player(PopHead::Base::GameData* gameData)
:Character(gameData, "player", 150)
{
    mSprite.setPosition(400, 400);
}

void Player::input()
{
    if(mGameData->getInput().getAction().isActionPressed("movingLeft")){
        mMotion.isMovingLeft = true;
    }
    if(mGameData->getInput().getAction().isActionPressed("movingRight")){
        mMotion.isMovingRight = true;
    }
    if(mGameData->getInput().getAction().isActionPressed("movingUp")){
        mMotion.isMovingUp = true;
    }
    if(mGameData->getInput().getAction().isActionPressed("movingDown")){
        mMotion.isMovingDown = true;
    }

    if(mGameData->getInput().getAction().isActionJustPressed("shot"));
        std::cout<<"just pressed"<<std::endl;
    if(mGameData->getInput().getAction().isActionJustReleased("shot"));
        std::cout<<"just released"<<std::endl;
}

void Player::update(sf::Time delta)
{
    sf::Vector2f velocity;

    if(mMotion.isMovingLeft){
        velocity.x -= mMovementSpeed * delta.asSeconds();
    }
    if(mMotion.isMovingRight){
        velocity.x += mMovementSpeed * delta.asSeconds();
    }
    if(mMotion.isMovingUp){
        velocity.y -= mMovementSpeed * delta.asSeconds();
    }
    if(mMotion.isMovingDown){
        velocity.y += mMovementSpeed * delta.asSeconds();
    }

    mSprite.move(velocity);

    mMotion.clear();
}

/*void talkTo(NPC&)
{

}*/

//auto getPerks() const -> const PerkManager& { return mPerks; }
