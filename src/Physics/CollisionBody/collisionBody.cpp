#include "collisionBody.hpp"
#include "bodyType.hpp"

namespace ph {

CollisionBody::CollisionBody(sf::FloatRect rect, float mass, BodyType bodyType, const std::string& name)
	:mRect(rect)
	,mPreviousPosition(rect.left, rect.top)
	,mVelocity(0, 0)
	,mForceVector(0, 0)
	,mMass(mass)
	,mBodyType(bodyType)
	,mName(name)
{
}

void CollisionBody::move(sf::Vector2f velocity)
{
	mVelocity = velocity;
	mRect.left += velocity.x;
	mRect.top += velocity.y;
}

void CollisionBody::setPosition(sf::Vector2f position)
{
	mRect.left = position.x;
	mRect.top = position.y;
}

void CollisionBody::actionsAtTheEndOfPhysicsLoopIteration()
{
	setPreviousPositionToCurrentPosition();
	mVelocity = sf::Vector2f();
}

void CollisionBody::setPreviousPositionToCurrentPosition()
{
	mPreviousPosition.x = mRect.left;
	mPreviousPosition.y = mRect.top;
}

void CollisionBody::updatePush(sf::Time delta)
{
	if (mForceVector == sf::Vector2f())
		return;

    move(mForceVector * delta.asSeconds());
    mForceVector -= mForceVector * delta.asSeconds() * 1.5f;

	if (mForceVector.x < 40 && mForceVector.x > -40 && mForceVector.y < 40 && mForceVector.y > -40) {
		mForceVector = sf::Vector2f(0, 0);
	}
}

}