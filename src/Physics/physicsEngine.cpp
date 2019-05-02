#include "physicsEngine.hpp"

#include "collisionBody.hpp"

using PopHead::Physics::PhysicsEngine;
using PopHead::Physics::CollisionBody;
using PopHead::Physics::CollisionAxis;

void PhysicsEngine::update(sf::Time delta)
{
    for(auto kinematicBody : mKinematicBodies)
    {
		kinematicBody->movePhysics();
		kinematicBody->setPositionOfGraphicRepresentation();
    }
}

void PhysicsEngine::addStaticBody(CollisionBody* staticBodyPtr)
{
    mStaticBodies.emplace_back(std::move(staticBodyPtr));
}

void PhysicsEngine::addKinematicBody(CollisionBody* kinematicBodyPtr)
{
    mKinematicBodies.emplace_back(std::move(kinematicBodyPtr));
}

void PhysicsEngine::removeStaticBody(CollisionBody* staticBodyPtr)
{
    for(auto it = mStaticBodies.begin(); it != mStaticBodies.end(); ++it){
        if(*it == staticBodyPtr){
            mStaticBodies.erase(it);
            break;
        }
    }
}

void PhysicsEngine::removeKinematicBody(CollisionBody* kinematicBodyPtr)
{
    for(auto it = mKinematicBodies.begin(); it != mKinematicBodies.end(); ++it){
        if(*it == kinematicBodyPtr){
            mKinematicBodies.erase(it);
            break;
        }
    }
}

void PhysicsEngine::clear() noexcept
{
	mStaticBodies.clear();
	mKinematicBodies.clear();
}

void PhysicsEngine::handleStaticCollisionsForThisKinematicBody(CollisionBody* kinematicBody) const
{

}

CollisionAxis PhysicsEngine::getAxisOfCollision(CollisionBody* kinematicBody, CollisionBody* staticBody)
{
	return CollisionAxis::none;
}

bool PhysicsEngine::isThereCollision(sf::FloatRect bodyA, sf::FloatRect bodyB)
{
	return false;
}

