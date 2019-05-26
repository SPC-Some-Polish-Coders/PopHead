#include "kinematicCollisionHandler.hpp"
#include "collisionBody.hpp"
#include "Utilities/debug.hpp"
#include <cmath>

using PopHead::Physics::KinematicCollisionHandler;
using PopHead::Physics::CollisionBody;


void KinematicCollisionHandler::operator()(CollisionBody* firstKinematicBody, CollisionBody* secondKinematicBody)
{
	init(firstKinematicBody, secondKinematicBody);
	collisionLog();
	calculateForceVector();
	applyForceVector();
}

void KinematicCollisionHandler::init(CollisionBody* firstKinematicBody, CollisionBody* secondKinematicBody)
{
	mFirstKinematicBody = firstKinematicBody;
	mSecondKinematicBody = secondKinematicBody;

	mMass1 = mFirstKinematicBody->getMass();
	mMass2 = mSecondKinematicBody->getMass();
}

void KinematicCollisionHandler::collisionLog() const
{
	PH_LOG(LogType::Info, "There is kinematic collision between " +
		mFirstKinematicBody->getNameOfOwner() + " and " + mSecondKinematicBody->getNameOfOwner() + ".");
}

void KinematicCollisionHandler::calculateForceVector()
{
	float force = getForce();
	sf::Vector2f directionOfPush = getDirectionOfPush();
	mForceVector = sf::Vector2f(force * directionOfPush.x, force * directionOfPush.y);
}

float KinematicCollisionHandler::getForce() const
{
	float force;
	constexpr float forceMultiplier = 12.5f;
	constexpr float theSmallestPossibleForce = 13 * forceMultiplier;

	if (mMass1 > mMass2)
		force = (mMass1 - mMass2) * forceMultiplier;
	else
		force = (mMass2 - mMass1) * forceMultiplier;

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
	float hypotenuse = std::hypotf(sides.x, sides.y);
	float sineResult = std::abs(sides.x / hypotenuse);

	directionOfPush.x = posOfBody1.x > posOfBody2.x ? sineResult : -1 * sineResult;
	directionOfPush.y = posOfBody1.y > posOfBody2.y ? 1 - sineResult : -1 * (1 - sineResult);

	return directionOfPush;
}

void KinematicCollisionHandler::applyForceVector() const
{
	if (mMass1 > mMass2)
		mSecondKinematicBody->setForceVector(mForceVector);
	else if (mMass1 < mMass2)
		mFirstKinematicBody->setForceVector(mForceVector);
	else
		applyForceVectorForBodiesOfEqualsMasses();
}

void KinematicCollisionHandler::applyForceVectorForBodiesOfEqualsMasses() const
{
	float velocitySumOfFirstBody = std::abs(mFirstKinematicBody->getVelocity().x) + std::abs(mFirstKinematicBody->getVelocity().y);
	float velocitySumOfSecondBody = std::abs(mSecondKinematicBody->getVelocity().x) + std::abs(mSecondKinematicBody->getVelocity().y);

	if (velocitySumOfFirstBody > velocitySumOfSecondBody)
		mSecondKinematicBody->setForceVector(mForceVector);
	else if (velocitySumOfFirstBody < velocitySumOfSecondBody)
		mFirstKinematicBody->setForceVector(mForceVector);
	else
		applyForceVectorForBodiesOfEqualsVelocitiesAndMasses();
}

void KinematicCollisionHandler::applyForceVectorForBodiesOfEqualsVelocitiesAndMasses() const
{
	sf::Vector2f halfForceVector = mForceVector;
	halfForceVector.x /= 2;
	halfForceVector.y /= 2;

	mFirstKinematicBody->setForceVector(halfForceVector);
	mSecondKinematicBody->setForceVector(-halfForceVector);
}
