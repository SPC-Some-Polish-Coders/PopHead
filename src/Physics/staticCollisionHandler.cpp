#include "staticCollisionHandler.hpp"

#include <SFML/Graphics.hpp>
#include "collisionBody.hpp"
#include "Utilities/math.hpp"

using PopHead::Physics::StaticCollisionHandler;
using PopHead::Physics::CollisionBody;

void StaticCollisionHandler::operator()(CollisionBody* kinematicBody, CollisionBody* staticBody)
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
	return (mKinematicBody->getPreviousRect().top + mKinematicBody->getPreviousRect().height > mStaticBody->getRect().top &&
		    mKinematicBody->getPreviousRect().top < mStaticBody->getRect().top + mStaticBody->getRect().height);
}

bool StaticCollisionHandler::isKinematicBodyOnTheLeftOfTheStaticBody()
{
	return mKinematicBody->getPreviousRect().left < mStaticBody->getRect().left;
}

void StaticCollisionHandler::stickToLeft()
{
	mKinematicBody->setPosition(sf::Vector2f(mStaticBody->getRect().left - mKinematicBody->getRect().width, mKinematicBody->getRect().top));
}

void StaticCollisionHandler::stickToRight()
{
	using namespace PopHead::Utilities::Math;
	mKinematicBody->setPosition(sf::Vector2f(getRightBound(mStaticBody->getRect()), mKinematicBody->getRect().top));
}

bool StaticCollisionHandler::isKinematicBodyUpOfTheStaticBody()
{
	return mKinematicBody->getPreviousRect().top < mStaticBody->getRect().top;
}

void StaticCollisionHandler::stickToTop()
{
	mKinematicBody->setPosition(sf::Vector2f(mKinematicBody->getRect().left, mStaticBody->getRect().top - mKinematicBody->getRect().height));
}

void StaticCollisionHandler::stickToBottom()
{
	using namespace PopHead::Utilities::Math;
	mKinematicBody->setPosition(sf::Vector2f(mKinematicBody->getRect().left, getBottomBound(mStaticBody->getRect())));
}

