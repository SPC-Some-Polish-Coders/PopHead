#pragma once

#include "GameObjects/DrawableGameObjects/character.hpp"
#include "Input/event.hpp"

namespace ph{

struct PlayerMotion
{
	PlayerMotion();
	bool isMoving();
	bool isMovingDiagonally();
	void clear();

	bool isMovingLeft;
	bool isMovingRight;
	bool isMovingUp;
	bool isMovingDown;
};

class Player : public Character
{
public:
    Player(GameData*);

	void handleEventOnCurrent(const ph::Event&) override;
    void updateCurrent(sf::Time delta) override;

	void slowDown() { mIsSlownDown = true; }
	float getPickRadius() const { return mPickRadius; }
	bool isDead() const { return mIsDead; }

private:
	void die();
	void updateCounters() const;
	void updateMovement(const sf::Time delta);
	void updateAnimation(const sf::Time delta);
    void setAnimationState(const std::string& stateName);
	void shootingUpdate(const sf::Time delta);
	void cameraMovement(sf::Time delta) const;
	void updateListenerPosition() const;

	sf::Vector2f getCurrentPlayerDirection() const;
	float getPlayerRotation() const;

private:
	static const sf::Time meleeAttackInterval;
	sf::Clock mTimeFromLastMeleeAttack;
	PlayerMotion mMotion;
	PlayerMotion mLastMotion;
	unsigned mNumberOfOwnedBullets;
	float mPickRadius;
	bool mIsSlownDown;
	bool mIsDead;
};

}
