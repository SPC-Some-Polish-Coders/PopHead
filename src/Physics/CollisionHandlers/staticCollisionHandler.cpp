#include "staticCollisionHandler.hpp"

#include "Physics/CollisionBody/collisionBody.hpp"
#include "Utilities/math.hpp"
#include "Utilities/debug.hpp"

using ph::Physics::StaticCollisionHandler;
using ph::Physics::CollisionBody;

void StaticCollisionHandler::operator()(CollisionBody* kinematicBody, CollisionBody* staticBody)
{
	init(kinematicBody, staticBody);
	collisionLog();
	makeKinematicBodyStickToStaticBody();
}

void StaticCollisionHandler::init(CollisionBody* kinematicBody, CollisionBody* staticBody)
{
	mKinematicBody = kinematicBody;
	mStaticBody = staticBody;

	mKinematicBodyRect = mKinematicBody->getRect();
	mStaticBodyRect = mStaticBody->getRect();
	mKinematicBodyPreviousRect = mKinematicBody->getPreviousRect();
	mStaticBodyPreviousRect = mStaticBody->getPreviousRect();
}

void StaticCollisionHandler::collisionLog() const
{
	PH_LOG(LogType::Info, "There is static collision between " +
		mKinematicBody->getNameOfOwner() + " and " + mStaticBody->getNameOfOwner() + ".");
}

void StaticCollisionHandler::makeKinematicBodyStickToStaticBody()
{
	if (isKinematicBodyCollidingOnAxisX()) {
		if (isKinematicBodyOnTheLeftOfTheStaticBody())
			stickToLeft();
		else
			stickToRight();
	}
	else {
		if (isKinematicBodyUpOfTheStaticBody())
			stickToTop();
		else
			stickToBottom();
	}
}

bool StaticCollisionHandler::isKinematicBodyCollidingOnAxisX()
{
	return (mKinematicBodyPreviousRect.top + mKinematicBodyPreviousRect.height > mStaticBodyRect.top &&
		    mKinematicBodyPreviousRect.top < mStaticBodyRect.top + mStaticBodyRect.height);
}

bool StaticCollisionHandler::isKinematicBodyOnTheLeftOfTheStaticBody()
{
	return mKinematicBodyPreviousRect.left < mStaticBodyRect.left;
}

void StaticCollisionHandler::stickToLeft()
{
	mKinematicBody->setPosition(sf::Vector2f(mStaticBodyRect.left - mKinematicBodyRect.width, mKinematicBodyRect.top));
}

void StaticCollisionHandler::stickToRight()
{
	using namespace ph::Utilities::Math;
	mKinematicBody->setPosition(sf::Vector2f(getRightBound(mStaticBodyRect), mKinematicBodyRect.top));
}

bool StaticCollisionHandler::isKinematicBodyUpOfTheStaticBody()
{
	return mKinematicBodyPreviousRect.top < mStaticBodyRect.top;
}

void StaticCollisionHandler::stickToTop()
{
	mKinematicBody->setPosition(sf::Vector2f(mKinematicBodyRect.left, mStaticBodyRect.top - mKinematicBodyRect.height));
}

void StaticCollisionHandler::stickToBottom()
{
	using namespace ph::Utilities::Math;
	mKinematicBody->setPosition(sf::Vector2f(mKinematicBodyRect.left, getBottomBound(mStaticBodyRect)));
}

