#include "collisionBody.hpp"
#include "bodyType.hpp"

using PopHead::Physics::CollisionBody;

CollisionBody::CollisionBody(sf::FloatRect rect, float mass, PopHead::Physics::BodyType bodyType,
							 PopHead::World::Entity::Object* const owner, PopHead::Base::GameData* gameData)
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
	case PopHead::Physics::BodyType::staticBody:
		gameData->getPhysicsEngine().addStaticBody(this);
		break;

	case PopHead::Physics::BodyType::kinematicBody:
		gameData->getPhysicsEngine().addKinematicBody(this);
		break;
	}
}

CollisionBody::~CollisionBody()
{
	switch (mBodyType)
	{
	case PopHead::Physics::BodyType::staticBody:
		mGameData->getPhysicsEngine().removeStaticBody(this);
		break;

	case PopHead::Physics::BodyType::kinematicBody:
		mGameData->getPhysicsEngine().removeKinematicBody(this);
		break;
	}
}

void CollisionBody::move(sf::Vector2f velocity)
{
	mRect.left += velocity.x;
	mRect.top += velocity.y;
	mCollisionDebugRect.move(velocity);
}

void CollisionBody::setPosition(sf::Vector2f position)
{
	mRect.left = position.x;
	mRect.top = position.y;
	mCollisionDebugRect.setPosition(position);
}

void CollisionBody::updateOwnerPosition()
{
	mOwner->setPosition(sf::Vector2f(mRect.left, mRect.top), false);
}

void CollisionBody::setPreviousPositionToCurrentPosition()
{
	mPreviousPosition.x = mRect.left;
	mPreviousPosition.y = mRect.top;
}

sf::FloatRect CollisionBody::getPreviousRect()
{
	return sf::FloatRect(mPreviousPosition.x, mPreviousPosition.y, mRect.width, mRect.height);
}

void CollisionBody::setForceVector(sf::Vector2f forceVector)
{
    this->forceVector = forceVector;
}

void CollisionBody::updatePush(sf::Time delta)
{
    move(forceVector * delta.asSeconds());

    forceVector -= forceVector * delta.asSeconds() * 1.5f;

	if(forceVector.x < 2 && forceVector.x > -2)
        forceVector.x = 0;
    if(forceVector.y < 2 && forceVector.y > -2)
        forceVector.y = 0;
}
