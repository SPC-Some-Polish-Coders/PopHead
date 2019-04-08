#include "player.hpp"

#include "Base/gameData.hpp"

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
	mAnimation.setDelay(sf::seconds(0.2f));
	mAnimation.animate(mSprite);
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
        mAnimation.changeState("left");
    }
    if(mMotion.isMovingRight){
        velocity.x += mMovementSpeed * delta.asSeconds();
        mAnimation.changeState("right");
    }
    if(mMotion.isMovingUp){
        velocity.y -= mMovementSpeed * delta.asSeconds();
        mAnimation.changeState("up");
    }
    if(mMotion.isMovingDown){
        velocity.y += mMovementSpeed * delta.asSeconds();
        mAnimation.changeState("down");
    }

    mSprite.move(velocity);

    mAnimation.animate(mSprite, delta);

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

/*auto getPerks() const -> const PerkManager&
{
    return mPerks;
}*/
