#include "physicsEngine.hpp"

#include "collisionBody.hpp"

#include <iostream>

using PopHead::Physics::PhysicsEngine;
using PopHead::Physics::CollisionBody;
using PopHead::Physics::CollisionAxis;
using PopHead::Physics::CollisionDebugSettings;


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

void PhysicsEngine::update(sf::Time delta)
{
    for(auto kinematicBody : mKinematicBodies)
    {
		handleStaticCollisionsFor(kinematicBody);
		kinematicBody->updateOwnerPosition();
		kinematicBody->setPreviousPositionToCurrentPosition();
    }
}

void PhysicsEngine::handleStaticCollisionsFor(CollisionBody* kinematicBody)
{
	for (const auto& staticBody : mStaticBodies) {
		CollisionAxis axis = getAxisOfCollision(kinematicBody, staticBody);
		if(axis != CollisionAxis::none)
			kinematicBody->setPositionToPreviousPosition(axis);
	}
}

auto PhysicsEngine::getAxisOfCollision(CollisionBody* kinematicBody, CollisionBody* staticBody) -> CollisionAxis
{
	if (isThereCollision(kinematicBody->mRect, staticBody->mRect))
	{
		if(isBodyBetweenTopAndBottomAxisesOfAnotherBody(kinematicBody, staticBody))
			return CollisionAxis::x;
		else
			return CollisionAxis::y;
	}
	else {
		return CollisionAxis::none;
	}
}

bool PhysicsEngine::isBodyBetweenTopAndBottomAxisesOfAnotherBody(CollisionBody* bodyA, CollisionBody* bodyB)
{
	return (bodyA->getPreviousRect().top + bodyA->getPreviousRect().height > bodyB->mRect.top &&
			bodyA->getPreviousRect().top < bodyB->mRect.top + bodyB->mRect.height);
}

bool PhysicsEngine::isThereCollision(sf::FloatRect A, sf::FloatRect B)
{
	//AABB collision detection algorithm
	return(
	A.left < B.left + B.width &&
	A.left + A.width > B.left &&
	A.top < B.top + B.height &&
	A.top + A.height > B.top);
}

