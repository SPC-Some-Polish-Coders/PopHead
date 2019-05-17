#include "kinematicCollisionHandler.hpp"
#include "collisionBody.hpp"

#include "Logs/logger.hpp"

using PopHead::Physics::KinematicCollisionHandler;
using PopHead::Physics::CollisionBody;


void KinematicCollisionHandler::handleKinematicCollision(CollisionBody* firstKinematicBody, CollisionBody* secondKinematicBody)
{
	init(firstKinematicBody, secondKinematicBody);
    applyKinematicCollision();
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
		return (mMass1 - mMass2) * forceMultiplier;
	}
	else {
		return (mMass2 - mMass1) * forceMultiplier;
	}
}

sf::Vector2f KinematicCollisionHandler::getDirectionOfPush() const
{
	sf::Vector2f posOfBody1 = mFirstKinematicBody->getPositionOfCenter();
	sf::Vector2f posOfBody2 = mSecondKinematicBody->getPositionOfCenter();

	sf::Vector2f sides = posOfBody1 - posOfBody2;

	sf::Vector2f directionOfPush;
	float sideC = sqrt(sides.x * sides.x + sides.y * sides.y);
	float div = sides.x / sideC;
	if (div < 0)
		div *= -1;

	directionOfPush.x = posOfBody1.x > posOfBody2.x ? div : -1 * div;
	directionOfPush.y = posOfBody1.y > posOfBody2.y ? 1 - div : -1 * (1 - div);

	return directionOfPush;
}

void KinematicCollisionHandler::applyKinematicCollision()
{
    calculateMomentums();
    if(mMomentum1 * 2 > mMomentum2 || mMomentum1 < mMomentum2 * 2)
        applyPush();
    else
        applyShift();

}

void KinematicCollisionHandler::calculateMomentums()
{
    mMomentum1 = mFirstKinematicBody->mSpeed > 1 ? mFirstKinematicBody->mSpeed * mMass1 : 1;
    mMomentum2 = mSecondKinematicBody->mSpeed > 1 ? mSecondKinematicBody->mSpeed * mMass2 : 1;

	PH_LOG(LogType::Info, "momentum 1: " + std::to_string(mMomentum1));
	PH_LOG(LogType::Info, "momentum 2: " + std::to_string(mMomentum2));
}

void KinematicCollisionHandler::applyPush() const
{
    float force = getForce();
	if (force == 0)
		return;
	sf::Vector2f directionOfPush = getDirectionOfPush();
    sf::Vector2f forceVector(force * directionOfPush.x, force * directionOfPush.y);

	if (mMass1 > mMass2)
		mSecondKinematicBody->setForceVector(forceVector);
	else
		mFirstKinematicBody->setForceVector(forceVector);
}

void KinematicCollisionHandler::applyShift() const
{

}
