#include "kinematicCollisionHandler.hpp"
#include "collisionBody.hpp"

#include "Logs/logger.hpp"

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
	if (mMass1 == mMass2) {
		return 0;
	}
	else if (mMass1 > mMass2) {
		return (mMass1 - mMass2) * 12.5;
	}
	else {
		return (mMass2 - mMass1) * 12.5;
	}
}

sf::Vector2f KinematicCollisionHandler::getDirectionOfPush() const
{
	sf::Vector2f kPos1 = mFirstKinematicBody->getPosition() + sf::Vector2f(16, 16);
	sf::Vector2f kPos2 = mSecondKinematicBody->getPosition() + sf::Vector2f(16, 16);

	sf::Vector2f sides = kPos1 - kPos2;

	sf::Vector2f directionOfPush;
	float sideC = sqrt(sides.x * sides.x + sides.y * sides.y);
	float div = sides.x / sideC;
	if (div < 0)
		div *= -1;

	directionOfPush.x = kPos1.x > kPos2.x ? div : -1 * div;
	directionOfPush.y = kPos1.y > kPos2.y ? 1 - div : -1 * (1 - div);

	return directionOfPush;
}


void KinematicCollisionHandler::applyForce(sf::Vector2f forceVector) const
{
	if (mMass1 > mMass2)
		mSecondKinematicBody->setForceVector(forceVector);
	else
		mFirstKinematicBody->setForceVector(forceVector);
}
