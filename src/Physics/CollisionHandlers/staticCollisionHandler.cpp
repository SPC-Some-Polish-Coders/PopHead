#include "staticCollisionHandler.hpp"

#include "Physics/CollisionBody/collisionBody.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"

namespace ph {

void StaticCollisionHandler::operator()(CollisionBody& kinematicBody, const CollisionBody& staticBody)
{
	init(kinematicBody, staticBody);
	makeKinematicBodyStickToStaticBody();
}

void StaticCollisionHandler::init(CollisionBody& kinematicBody, const CollisionBody& staticBody)
{
	mKinematicBody = &kinematicBody;
	mStaticBody = &staticBody;

	mKinematicBodyRect = mKinematicBody->getRect();
	mStaticBodyRect = mStaticBody->getRect();
	mKinematicBodyPreviousRect = mKinematicBody->getPreviousRect();
	//mStaticBodyPreviousRect = mStaticBody->getPreviousRect();
}

void StaticCollisionHandler::makeKinematicBodyStickToStaticBody()
{
	if(isKinematicBodyCollidingOnAxisX()) {
		if(isKinematicBodyOnTheLeftOfTheStaticBody())
			stickToLeft();
		else
			stickToRight();
	}
	else if(isKinematicBodyCollidingOnAxisY()){
		if(isKinematicBodyUpOfTheStaticBody())
			stickToTop();
		else
			stickToBottom();
	}
}

bool StaticCollisionHandler::isKinematicBodyCollidingOnAxisX() const
{
	return (Math::getBottomBound(mKinematicBodyPreviousRect) > mStaticBodyRect.top &&
		mKinematicBodyPreviousRect.top < Math::getBottomBound(mStaticBodyRect));
}

bool StaticCollisionHandler::isKinematicBodyOnTheLeftOfTheStaticBody() const
{
	return mKinematicBodyPreviousRect.left < mStaticBodyRect.left;
}

void StaticCollisionHandler::stickToLeft()
{
	mKinematicBody->setPosition({mStaticBodyRect.left - mKinematicBodyRect.width, mKinematicBodyRect.top});
}

void StaticCollisionHandler::stickToRight()
{
	mKinematicBody->setPosition({Math::getRightBound(mStaticBodyRect), mKinematicBodyRect.top});
}

bool StaticCollisionHandler::isKinematicBodyCollidingOnAxisY() const
{
	return (Math::getRightBound(mKinematicBodyPreviousRect) > mStaticBodyRect.left &&
		mKinematicBodyPreviousRect.left < Math::getRightBound(mStaticBodyRect));
}

bool StaticCollisionHandler::isKinematicBodyUpOfTheStaticBody() const
{
	return mKinematicBodyPreviousRect.top < mStaticBodyRect.top;
}

void StaticCollisionHandler::stickToTop()
{
	mKinematicBody->setPosition({mKinematicBodyRect.left, mStaticBodyRect.top - mKinematicBodyRect.height});
}

void StaticCollisionHandler::stickToBottom()
{
	mKinematicBody->setPosition({mKinematicBodyRect.left, Math::getBottomBound(mStaticBodyRect)});
}

}