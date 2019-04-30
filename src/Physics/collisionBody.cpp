#include "collisionBody.hpp"
#include "bodyType.hpp"

using PopHead::Physics::CollisionBody;

CollisionBody::CollisionBody(sf::FloatRect rect, float mass, PopHead::Physics::BodyType bodyType, PopHead::Base::GameData* gameData)
:mRect(rect)
,mMass(mass)
,mVelocity()
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
	mVelocity += velocity;
}

void CollisionBody::movePhysics()
{
	mRect.left += mVelocity.x;
	mRect.top += mVelocity.y;
}

void CollisionBody::setPositionOfGraphicRepresentation(sf::Vector2f position)
{

}
