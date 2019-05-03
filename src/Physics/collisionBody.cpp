#include "collisionBody.hpp"
#include "bodyType.hpp"
#include "collisionAxis.hpp"

using PopHead::Physics::CollisionBody;
using PopHead::Physics::CollisionAxis;

CollisionBody::CollisionBody(sf::FloatRect rect, float mass, PopHead::Physics::BodyType bodyType,
							 PopHead::World::Entity::Object* const owner, PopHead::Base::GameData* gameData)
:mRect(rect)
,mMass(mass)
,mPreviousPosition(rect.left, rect.top)
,mOwner(owner)
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

void CollisionBody::move(sf::Vector2f velocity)
{
	mRect.left += velocity.x;
	mRect.top += velocity.y;
}

void CollisionBody::setPosition(sf::Vector2f position)
{
	mRect.left = position.x;
	mRect.top = position.y;
}

void CollisionBody::updateOwnerPosition()
{
	mOwner->setPosition(sf::Vector2f(mRect.left, mRect.top), false);
}

void CollisionBody::setPositionToPreviousPosition(CollisionAxis axis)
{
	/*mRect.left = mPreviousPosition.x;
	mRect.top = mPreviousPosition.y;*/

	switch (axis)
	{
	case CollisionAxis::x:
		mRect.left = mPreviousPosition.x;
		break;

	case CollisionAxis::y:
		mRect.top = mPreviousPosition.y;
		break;
	}
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
