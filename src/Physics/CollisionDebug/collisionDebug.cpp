#include "collisionDebug.hpp"

#include "Physics/bodyType.hpp"
#include "Physics/collisionBody.hpp"

using PopHead::Physics::CollisionDebug;
using PopHead::Physics::CollisionBody;
using PopHead::Physics::BodyType;


void CollisionDebug::createFrom(std::vector<CollisionBody*>& kinematicBodies,
                                std::vector<CollisionBody*>& staticBodies)
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
