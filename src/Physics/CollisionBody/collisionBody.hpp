#pragma once

#include <SFML/Graphics.hpp>
#include "Utilities/math.hpp"

namespace ph {

class CollisionBody
{
public:
	CollisionBody(sf::FloatRect rect, float mass);

	//the methods below should be called from owner
	void move(sf::Vector2f velocity);
	void setPosition(sf::Vector2f position);
	bool isBeingPushed() { return (mForceVector.x != 0 || mForceVector.y != 0); }

	//the methods below should be called only from physics module
	void updatePush(sf::Time delta);
	void setForceVector(sf::Vector2f forceVector) { mForceVector = forceVector; }
	
	void actionsAtTheEndOfPhysicsLoopIteration();
private:
	void setPreviousPositionToCurrentPosition();

public:
	auto getPosition() const -> const sf::Vector2f { return sf::Vector2f(mRect.left, mRect.top); }
	auto getVelocity() const -> sf::Vector2f { return mVelocity; }
	auto getPositionOfCenter() const -> sf::Vector2f { return ph::Math::getCenter(mRect); }
	auto getRect() const -> const sf::FloatRect& { return mRect; }
	auto getPreviousRect() const -> sf::FloatRect { return sf::FloatRect(mPreviousPosition.x, mPreviousPosition.y, mRect.width, mRect.height); }
	float getMass() const { return mMass; }

private:
	sf::FloatRect mRect;
	sf::Vector2f mPreviousPosition;
	sf::Vector2f mVelocity;
	sf::Vector2f mForceVector;
	float mMass;
};

}
