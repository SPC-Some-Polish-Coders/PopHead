#include "staticCollisionHandler.hpp"

#include <SFML/Graphics.hpp>
#include "collisionBody.hpp"

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
		if (mKinematicBody->getPreviousRect().left < mStaticBody->mRect.left) // left
			mKinematicBody->setPosition(sf::Vector2f(mStaticBody->mRect.left - mKinematicBody->mRect.width, mKinematicBody->mRect.top));
		else // right
			mKinematicBody->setPosition(sf::Vector2f(mStaticBody->mRect.left + mStaticBody->mRect.width, mKinematicBody->mRect.top));
	}
	else {
		if (mKinematicBody->getPreviousRect().top < mStaticBody->mRect.top) // up
			mKinematicBody->setPosition(sf::Vector2f(mKinematicBody->mRect.left, mStaticBody->mRect.top - mKinematicBody->mRect.height));
		else // down
			mKinematicBody->setPosition(sf::Vector2f(mKinematicBody->mRect.left, mStaticBody->mRect.top + mStaticBody->mRect.height));
	}
}

bool StaticCollisionHandler::isKinematicBodyCollidingOnAxisX()
{
	return (mKinematicBody->getPreviousRect().top + mKinematicBody->getPreviousRect().height > mStaticBody->mRect.top &&
		    mKinematicBody->getPreviousRect().top < mStaticBody->mRect.top + mStaticBody->mRect.height);
}
