#include "collisionBody.hpp"

namespace ph {

CollisionBody::CollisionBody(const sf::FloatRect& rect, const float mass)
	:mRect(rect)
	,mPreviousPosition(rect.left, rect.top)
	,mVelocity(0, 0)
	,mForceVector(0, 0)
	,mMass(mass)
{
}

void CollisionBody::move(const sf::Vector2f velocity)
{
	mVelocity = velocity;
	mRect.left += velocity.x;
	mRect.top += velocity.y;
}

void CollisionBody::setPosition(const sf::Vector2f position)
{
	mRect.left = position.x;
	mRect.top = position.y;
}

bool CollisionBody::isBeingPushed() const
{
	return (mForceVector.x != 0 || mForceVector.y != 0);
}

void CollisionBody::actionsAtTheEndOfPhysicsLoopIteration()
{
	setPreviousPositionToCurrentPosition();
	mVelocity = sf::Vector2f();
}

void CollisionBody::setPreviousPositionToCurrentPosition()
{
	mPreviousPosition = getPosition();
}

auto CollisionBody::getPosition() const -> const sf::Vector2f
{
	return mRect.getTopLeft();
}

sf::Vector2f CollisionBody::getFixedPosition() const
{
	return { mRect.left, mRect.top - mRect.height };
}

auto CollisionBody::getPositionOfCenter() const -> sf::Vector2f
{
	return mRect.getCenter();
}

auto CollisionBody::getPreviousRect() const -> sf::FloatRect
{
	return { mPreviousPosition.x, mPreviousPosition.y, mRect.width, mRect.height };
}

void CollisionBody::updatePush(const sf::Time delta)
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