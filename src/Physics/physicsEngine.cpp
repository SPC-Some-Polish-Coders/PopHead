#include "physicsEngine.hpp"

using PopHead::Physics::PhysicsEngine;

void PhysicsEngine::update(sf::Time delta)
{

}

void PhysicsEngine::addStaticBody(CollisionBody* staticBodyPtr)
{
    mStaticBodies.emplace_back(staticBodyPtr);
}

void PhysicsEngine::addKinematicBody(CollisionBody* kinematicBodyPtr)
{
    mKinematicBodies.emplace_back(kinematicBodyPtr);
}

void PhysicsEngine::removeStaticBody(CollisionBody* staticBodyPtr)
{

}

void PhysicsEngine::removeKinematicBody(CollisionBody* kinematicBodyPtr)
{

}

void PhysicsEngine::clear() noexcept
{

}
