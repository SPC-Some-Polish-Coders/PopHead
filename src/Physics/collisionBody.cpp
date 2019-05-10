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
{
	switch (bodyType)
	{
	case PopHead::Physics::BodyType::staticBody:
		gameData->getPhysicsEngine().addStaticBody(this);
		mCollisionDebugRect.setColor(sf::Color(200, 0, 0, 100));
		break;

	case PopHead::Physics::BodyType::kinematicBody:
		gameData->getPhysicsEngine().addKinematicBody(this);
		mCollisionDebugRect.setColor(sf::Color(45, 100, 150, 135));
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

sf::FloatRect CollisionBody::getPreviousRect()
{
	return sf::FloatRect(mPreviousPosition.x, mPreviousPosition.y, mRect.width, mRect.height);
}

void PopHead::Physics::CollisionBody::setPreviousPositionToCurrentPosition()
{
	mPreviousPosition.x = mRect.left;
	mPreviousPosition.y = mRect.top;
}
