#include "collisionDebug.hpp"

#include "Physics/bodyType.hpp"

using PopHead::Physics::CollisionDebug;


void CollisionDebug::createFrom(const std::vector<CollisionBody*>& kinematicBodies,
                                const std::vector<CollisionBody*>& staticBodies)
{

}

void CollisionDebug::createFrom(const CollisionBody* collisionBody, BodyType type)
{
//    switch (bodyType)
//	{
//	case PopHead::Physics::BodyType::staticBody:
//		gameData->getPhysicsEngine().addStaticBody(this);
//		break;
//
//	case PopHead::Physics::BodyType::kinematicBody:
//		gameData->getPhysicsEngine().addKinematicBody(this);
//		break;
//	}
}

void CollisionDebug::clear()
{

}
