#include "staticCollisionHandler.hpp"

using PopHead::Physics::StaticCollisionHandler;
using PopHead::Physics::CollisionBody;

void StaticCollisionHandler::handleStaticCollision(CollisionBody* kinematicBody, CollisionBody* staticBody)
{
}

void StaticCollisionHandler::setToContactPosition()
{
}

bool StaticCollisionHandler::WouldKinematicBodyCollideOnAxisX()
{
	return false;
}
