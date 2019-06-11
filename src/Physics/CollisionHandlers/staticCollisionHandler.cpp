#include "staticCollisionHandler.hpp"

#include "Physics/CollisionBody/collisionBody.hpp"
#include "Utilities/math.hpp"
#include "Utilities/debug.hpp"

void ph::StaticCollisionHandler::operator()(CollisionBody* kinematicBody, CollisionBody* staticBody)
{
	init(kinematicBody, staticBody);
	collisionLog();
	makeKinematicBodyStickToStaticBody();
}

void ph::StaticCollisionHandler::init(CollisionBody* kinematicBody, CollisionBody* staticBody)
{
	mKinematicBody = kinematicBody;
	mStaticBody = staticBody;

	mKinematicBodyRect = mKinematicBody->getRect();
	mStaticBodyRect = mStaticBody->getRect();
	mKinematicBodyPreviousRect = mKinematicBody->getPreviousRect();
	mStaticBodyPreviousRect = mStaticBody->getPreviousRect();
}

void ph::StaticCollisionHandler::collisionLog() const
{
	PH_LOG(LogType::Info, "There is static collision between " +
		mKinematicBody->getNameOfOwner() + " and " + mStaticBody->getNameOfOwner() + ".");
}

void ph::StaticCollisionHandler::makeKinematicBodyStickToStaticBody()
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

bool ph::StaticCollisionHandler::isKinematicBodyCollidingOnAxisX()
{
	return (mKinematicBodyPreviousRect.top + mKinematicBodyPreviousRect.height > mStaticBodyRect.top &&
		    mKinematicBodyPreviousRect.top < mStaticBodyRect.top + mStaticBodyRect.height);
}

bool ph::StaticCollisionHandler::isKinematicBodyOnTheLeftOfTheStaticBody()
{
	return mKinematicBodyPreviousRect.left < mStaticBodyRect.left;
}

void ph::StaticCollisionHandler::stickToLeft()
{
	mKinematicBody->setPosition(sf::Vector2f(mStaticBodyRect.left - mKinematicBodyRect.width, mKinematicBodyRect.top));
}

void ph::StaticCollisionHandler::stickToRight()
{
	mKinematicBody->setPosition(sf::Vector2f(Math::getRightBound(mStaticBodyRect), mKinematicBodyRect.top));
}

bool ph::StaticCollisionHandler::isKinematicBodyUpOfTheStaticBody()
{
	return mKinematicBodyPreviousRect.top < mStaticBodyRect.top;
}

void ph::StaticCollisionHandler::stickToTop()
{
	mKinematicBody->setPosition(sf::Vector2f(mKinematicBodyRect.left, mStaticBodyRect.top - mKinematicBodyRect.height));
}

void ph::StaticCollisionHandler::stickToBottom()
{
	mKinematicBody->setPosition(sf::Vector2f(mKinematicBodyRect.left, Math::getBottomBound(mStaticBodyRect)));
}
