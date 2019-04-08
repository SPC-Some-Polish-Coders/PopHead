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
    if(mGameData->getInput().getKeyboard().isKeyPressed(sf::Keyboard::A)){
        mMotion.isMovingLeft = true;
    }
    if(mGameData->getInput().getKeyboard().isKeyPressed(sf::Keyboard::D)){
        mMotion.isMovingRight = true;
    }
    if(mGameData->getInput().getKeyboard().isKeyPressed(sf::Keyboard::W)){
        mMotion.isMovingUp = true;
    }
    if(mGameData->getInput().getKeyboard().isKeyPressed(sf::Keyboard::S)){
        mMotion.isMovingDown = true;
    }
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
