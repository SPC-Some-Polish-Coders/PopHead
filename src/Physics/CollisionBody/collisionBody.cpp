#include "collisionBody.hpp"
#include "bodyType.hpp"

ph::CollisionBody::CollisionBody(sf::FloatRect rect, float mass, BodyType bodyType,
								Object* const owner, GameData* gameData)
	:mRect(rect)
	,mMass(mass)
	,mPreviousPosition(rect.left, rect.top)
	,mOwner(owner)
	,mBodyType(bodyType)
	,mCollisionDebugRect(gameData, rect, this)
	,mGameData(gameData)
{
	switch (bodyType)
	{
	case BodyType::staticBody:
		gameData->getPhysicsEngine().addStaticBody(this);
		break;

	case BodyType::kinematicBody:
		gameData->getPhysicsEngine().addKinematicBody(this);
		break;
	}
}

ph::CollisionBody::~CollisionBody()
{
	switch (mBodyType)
	{
	case BodyType::staticBody:
		mGameData->getPhysicsEngine().removeStaticBody(this);
		break;

	case BodyType::kinematicBody:
		mGameData->getPhysicsEngine().removeKinematicBody(this);
		break;
	}
}

void ph::CollisionBody::move(sf::Vector2f velocity)
{
	mVelocity = velocity;
	mRect.left += velocity.x;
	mRect.top += velocity.y;
	mCollisionDebugRect.move(velocity);
}

void ph::CollisionBody::setPosition(sf::Vector2f position)
{
	mRect.left = position.x;
	mRect.top = position.y;
	mCollisionDebugRect.setPosition(position);
}

void ph::CollisionBody::actionsAtTheEndOfPhysicsLoopIteration()
{
	setPreviousPositionToCurrentPosition();
	updateOwnerPosition();
	mVelocity = sf::Vector2f();
}

void ph::CollisionBody::setPreviousPositionToCurrentPosition()
{
	mPreviousPosition.x = mRect.left;
	mPreviousPosition.y = mRect.top;
}

void ph::CollisionBody::updateOwnerPosition()
{
	mOwner->setPosition(sf::Vector2f(mRect.left, mRect.top), false);
}

void ph::CollisionBody::updatePush(sf::Time delta)
{
	if (mForceVector == sf::Vector2f())
		return;

    move(mForceVector * delta.asSeconds());
    mForceVector -= mForceVector * delta.asSeconds() * 1.5f;

	if (mForceVector.x < 40 && mForceVector.x > -40 && mForceVector.y < 40 && mForceVector.y > -40) {
		mForceVector = sf::Vector2f(0, 0);
	}
}
