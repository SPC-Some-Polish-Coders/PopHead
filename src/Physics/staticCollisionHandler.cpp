#include "staticCollisionHandler.hpp"

#include <SFML/Graphics.hpp>
#include "collisionBody.hpp"
#include "Utilities/math.hpp"

using PopHead::Physics::StaticCollisionHandler;
using PopHead::Physics::CollisionBody;

void StaticCollisionHandler::handleStaticCollision(CollisionBody* kinematicBody, CollisionBody* staticBody)
{
	mKinematicBody = kinematicBody;
	mStaticBody = staticBody;

	makeKinematicBodyStickToStaticBody();
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
	return (mKinematicBody->getPreviousRect().top + mKinematicBody->getPreviousRect().height > mStaticBody->mRect.top &&
		    mKinematicBody->getPreviousRect().top < mStaticBody->mRect.top + mStaticBody->mRect.height);
}

bool StaticCollisionHandler::isKinematicBodyOnTheLeftOfTheStaticBody()
{
	return mKinematicBody->getPreviousRect().left < mStaticBody->mRect.left;
}

void StaticCollisionHandler::stickToLeft()
{
	mKinematicBody->setPosition(sf::Vector2f(mStaticBody->mRect.left - mKinematicBody->mRect.width, mKinematicBody->mRect.top));
}

void StaticCollisionHandler::stickToRight()
{
	using namespace PopHead::Utilities::Math;
	mKinematicBody->setPosition(sf::Vector2f(getRightBound(mStaticBody->mRect), mKinematicBody->mRect.top));
}

bool StaticCollisionHandler::isKinematicBodyUpOfTheStaticBody()
{
	return mKinematicBody->getPreviousRect().top < mStaticBody->mRect.top;
}

void StaticCollisionHandler::stickToTop()
{
	mKinematicBody->setPosition(sf::Vector2f(mKinematicBody->mRect.left, mStaticBody->mRect.top - mKinematicBody->mRect.height));
}

void StaticCollisionHandler::stickToBottom()
{
	using namespace PopHead::Utilities::Math;
	mKinematicBody->setPosition(sf::Vector2f(mKinematicBody->mRect.left, getBottomBound(mStaticBody->mRect)));
}

