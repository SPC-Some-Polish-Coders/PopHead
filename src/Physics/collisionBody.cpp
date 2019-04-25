#include "collisionBody.hpp"

using PopHead::Physics::CollisionBody;

CollisionBody::CollisionBody(sf::FloatRect rect, float mass, BodyType bodyType, PopHead::Base::GameData* gameData)
:mRect(rect)
,mMass(mass)
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
	mVelocity.x += velocity.x;
	mVelocity.y += velocity.y;
}
