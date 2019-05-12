#include "physicsEngine.hpp"

#include "collisionBody.hpp"
#include "Utilities/math.hpp"

#include <iostream>

using PopHead::Physics::PhysicsEngine;
using PopHead::Physics::CollisionBody;

void PhysicsEngine::addStaticBody(CollisionBody* staticBodyPtr)
{
    mStaticBodies.emplace_back(std::move(staticBodyPtr));
}

void PhysicsEngine::addKinematicBody(CollisionBody* kinematicBodyPtr)
{
    mKinematicBodies.emplace_back(std::move(kinematicBodyPtr));
}

void PhysicsEngine::removeStaticBody(CollisionBody* staticBody)
{
	removeBody(mStaticBodies, staticBody);
}

void PhysicsEngine::removeKinematicBody(CollisionBody* kinematicBody)
{
	removeBody(mKinematicBodies, kinematicBody);
}

void PhysicsEngine::removeBody(std::vector<CollisionBody*>& bodies, CollisionBody* body)
{
	for (auto it = bodies.begin(); it != bodies.end(); ++it) {
		if (*it == body) {
			bodies.erase(it);
			break;
		}
	}
}

void PhysicsEngine::clear() noexcept
{
	mStaticBodies.clear();
	mKinematicBodies.clear();
}

void PhysicsEngine::update(sf::Time delta)
{
    for(auto kinematicBody : mKinematicBodies)
    {
		handleStaticCollisionsFor(kinematicBody);
		handleKinematicCollisionsFor(kinematicBody);
		kinematicBody->updatePush(delta);
		kinematicBody->updateOwnerPosition();
		kinematicBody->setPreviousPositionToCurrentPosition();
    }
}

void PhysicsEngine::handleStaticCollisionsFor(CollisionBody* kinematicBody)
{
	for (const auto& staticBody : mStaticBodies) {
		if (isThereCollision(kinematicBody->mRect, staticBody->mRect))
			mStaticCollisionHandler.handleStaticCollision(kinematicBody, staticBody);
	}
}

bool PhysicsEngine::isThereCollision(sf::FloatRect A, sf::FloatRect B)
{
	using namespace PopHead::Utilities::Math;

	//AABB collision detection algorithm
	return(
	A.left < getRightBound(B) &&
	getRightBound(A) > B.left &&
	A.top < getBottomBound(B) &&
	getBottomBound(A) > B.top);
}

void PhysicsEngine::handleKinematicCollisionsFor(CollisionBody* kinematicBody)
{
    for (const auto& tkinematicBody : mKinematicBodies)
    {
        if(isThereCollision(kinematicBody->mRect,tkinematicBody->mRect))
            mKinematicCollisionHandler.handleKinematicCollision(kinematicBody, tkinematicBody);
    }
}
