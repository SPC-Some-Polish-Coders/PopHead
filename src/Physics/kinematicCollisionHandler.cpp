#include "kinematicCollisionHandler.hpp"
#include "collisionBody.hpp"
#include <cmath>

using PopHead::Physics::KinematicCollisionHandler;
using PopHead::Physics::CollisionBody;


void KinematicCollisionHandler::handleKinematicCollision(CollisionBody* firstKinematicBody, CollisionBody* secondKinematicBody)
{
	init(firstKinematicBody, secondKinematicBody);
    
	float force = getForce();
	if (force == 0)
		return;
	sf::Vector2f directionOfPush = getDirectionOfPush();
	sf::Vector2f forceVector(force * directionOfPush.x, force * directionOfPush.y);

	applyForce(forceVector);
}

void KinematicCollisionHandler::init(CollisionBody* firstKinematicBody, CollisionBody* secondKinematicBody)
{
	mFirstKinematicBody = firstKinematicBody;
	mSecondKinematicBody = secondKinematicBody;

	mMass1 = mFirstKinematicBody->getMass();
	mMass2 = mSecondKinematicBody->getMass();
}

float KinematicCollisionHandler::getForce() const
{
	float force;
	constexpr float forceMultiplier = 12.5f;

	if (mMass1 > mMass2) {
		force = (mMass1 - mMass2) * forceMultiplier;
	}
	else {
		force = (mMass2 - mMass1) * forceMultiplier;
	}

	constexpr float theSmallestPossibleForce = 13 * forceMultiplier;
	if (force < theSmallestPossibleForce)
		force = theSmallestPossibleForce;

	return force;
}

sf::Vector2f KinematicCollisionHandler::getDirectionOfPush() const
{
	sf::Vector2f posOfBody1 = mFirstKinematicBody->getPositionOfCenter();
	sf::Vector2f posOfBody2 = mSecondKinematicBody->getPositionOfCenter();

	sf::Vector2f sides = posOfBody1 - posOfBody2;

	sf::Vector2f directionOfPush;
	float hypotenuse = sqrt(sides.x * sides.x + sides.y * sides.y);
	float sineResult = std::abs(sides.x / hypotenuse);

	directionOfPush.x = posOfBody1.x > posOfBody2.x ? sineResult : -1 * sineResult;
	directionOfPush.y = posOfBody1.y > posOfBody2.y ? 1 - sineResult : -1 * (1 - sineResult);

	return directionOfPush;
}

void KinematicCollisionHandler::applyForce(const sf::Vector2f& forceVector) const
{
	if (mMass1 > mMass2)
		mSecondKinematicBody->setForceVector(forceVector);
	else
		mFirstKinematicBody->setForceVector(forceVector);
}
