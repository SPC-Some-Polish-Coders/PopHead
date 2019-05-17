#include "physicsEngine.hpp"

#include "collisionBody.hpp"
#include "Utilities/math.hpp"
#include "Logs/logger.hpp"

using PopHead::Physics::PhysicsEngine;
using PopHead::Physics::CollisionBody;

void PhysicsEngine::addStaticBody(CollisionBody* staticBody)
{
    mStaticBodies.emplace_back(std::move(staticBody));
	PH_LOG(LogType::Info, "Static collision body of " + staticBody->mOwner->getName() + " was added to physics engine.");
}

void PhysicsEngine::addKinematicBody(CollisionBody* kinematicBody)
{
    mKinematicBodies.emplace_back(std::move(kinematicBody));
	PH_LOG(LogType::Info, "Kinematic collision body of " + kinematicBody->mOwner->getName() + " was added to physics engine.");
}

void PhysicsEngine::removeStaticBody(CollisionBody* staticBody)
{
	removeBody(mStaticBodies, staticBody);
	PH_LOG(LogType::Info, "Static collision body of " + staticBody->mOwner->getName() + " was deleted from physics engine.");
}

void PhysicsEngine::removeKinematicBody(CollisionBody* kinematicBody)
{
	removeBody(mKinematicBodies, kinematicBody);
	PH_LOG(LogType::Info, "Kinematic collision body of " + kinematicBody->mOwner->getName() + " was deleted from physics engine.");
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
		handleKinematicCollisionsFor(kinematicBody);
		kinematicBody->updatePush(delta);
		handleStaticCollisionsFor(kinematicBody);

		kinematicBody->setPreviousPositionToCurrentPosition();
		kinematicBody->updateOwnerPosition();
		kinematicBody->isMoving = false;
    }
}

void PhysicsEngine::handleStaticCollisionsFor(CollisionBody* kinematicBody)
{
	for (const auto& staticBody : mStaticBodies) {
		if (isThereCollision(kinematicBody->mRect, staticBody->mRect))
			mStaticCollisionHandler.handleStaticCollision(kinematicBody, staticBody);
	}
}

void PhysicsEngine::handleKinematicCollisionsFor(CollisionBody* kinematicBody)
{
    for (const auto& kinematicBody2 : mKinematicBodies)
    {
		if (kinematicBody == kinematicBody2)
			continue;

		if (isThereCollision(kinematicBody->mRect, kinematicBody2->mRect)) {
			PH_LOG(LogType::Info, "there is kinematic collision between " + kinematicBody->mOwner->getName() + " and " + kinematicBody2->mOwner->getName());
            mKinematicCollisionHandler.handleKinematicCollision(kinematicBody, kinematicBody2);
		}
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
