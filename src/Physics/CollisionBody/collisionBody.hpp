#pragma once

#include <SFML/Graphics.hpp>
#include "Utilities/math.hpp"

namespace ph {

class CollisionBody
{
public:
	CollisionBody(const sf::FloatRect& rect, const float mass);

	//the methods below should be called from owner
	void move(const sf::Vector2f velocity);
	void setPosition(const sf::Vector2f position);
	bool isBeingPushed() { return (mForceVector.x != 0 || mForceVector.y != 0); }

	//the methods below should be called only from physics module
	void updatePush(const sf::Time delta);
	void setForceVector(sf::Vector2f forceVector) { mForceVector = forceVector; }
	
	void actionsAtTheEndOfPhysicsLoopIteration();
private:
	void setPreviousPositionToCurrentPosition();

public:
	auto getPosition() const -> const sf::Vector2f { return Math::getTopLeftCorner(mRect); }
	auto getVelocity() const -> sf::Vector2f { return mVelocity; }
	auto getPositionOfCenter() const -> sf::Vector2f { return Math::getCenter(mRect); }
	auto getRect() const -> const sf::FloatRect& { return mRect; }
	auto getPreviousRect() const -> sf::FloatRect { return {mPreviousPosition.x, mPreviousPosition.y, mRect.width, mRect.height}; }
	float getMass() const { return mMass; }

private:
	sf::FloatRect mRect;
	sf::Vector2f mPreviousPosition;
	sf::Vector2f mVelocity;
	sf::Vector2f mForceVector;
	float mMass;
};

}
