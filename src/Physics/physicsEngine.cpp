#include "physicsEngine.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"
#include <memory>

namespace ph {

PhysicsEngine::PhysicsEngine()
{
	mStaticBodies.reserve(300);
}

CollisionBody& PhysicsEngine::createStaticBodyAndGetTheReference(const sf::FloatRect rect)
{
	mStaticBodies.emplace_back(std::make_unique<CollisionBody>(rect, 0.f));
	auto& staticBody = *mStaticBodies.back().get();
	mCollisionDebugManager.addStaticBodyCollisionDebugRect(staticBody);
	return staticBody;
}

CollisionBody& PhysicsEngine::createKinematicBodyAndGetTheReference(const sf::FloatRect rect, const float mass)
{
	mKinematicBodies.emplace_back(rect, mass);
	auto& kinematicBody = mKinematicBodies.back();
	mCollisionDebugManager.addKinematicBodyCollisionDebugRect(kinematicBody);
	return kinematicBody;
}

void PhysicsEngine::removeStaticBody(const CollisionBody& bodyToDelete)
{
	for(auto it = mStaticBodies.begin(); it != mStaticBodies.end(); ++it)
		if(it->get() == std::addressof(bodyToDelete)) {
			mStaticBodies.erase(it);
			break;
		}
}

void PhysicsEngine::removeKinematicBody(const CollisionBody& bodyToDelete)
{
	for(auto it = mKinematicBodies.begin(); it != mKinematicBodies.end(); ++it)
		if(std::addressof(*it) == std::addressof(bodyToDelete)) {
			mKinematicBodies.erase(it);
			break;
		}
}

void PhysicsEngine::clear() noexcept
{
	mStaticBodies.clear();
	mKinematicBodies.clear();
	mCollisionDebugManager.clear();
}

void PhysicsEngine::update(sf::Time delta)
{
    for(auto &kinematicBody : mKinematicBodies)
    {
		handleKinematicCollisionsFor(kinematicBody);
		kinematicBody.updatePush(delta);
		handleStaticCollisionsFor(kinematicBody);
		kinematicBody.actionsAtTheEndOfPhysicsLoopIteration();
    }
	updatePositionsOfDebugRects();
}

void PhysicsEngine::handleStaticCollisionsFor(CollisionBody& kinematicBody)
{
	for (auto& staticBody : mStaticBodies)
		if (isThereCollision(kinematicBody, *staticBody))
			mStaticCollisionHandler(kinematicBody, *staticBody);
}

void PhysicsEngine::handleKinematicCollisionsFor(CollisionBody& kinematicBody)
{
    for (auto& kinematicBody2 : mKinematicBodies)
    {
		if (std::addressof(kinematicBody) == std::addressof(kinematicBody2))
			continue;

		if (isThereCollision(kinematicBody, kinematicBody2))
            mKinematicCollisionHandler(kinematicBody, kinematicBody2);
    }
}

bool PhysicsEngine::isThereCollision(const CollisionBody& a, const CollisionBody& b) const
{
	return Math::areTheyOverlapping(a.getRect(), b.getRect());
}

void PhysicsEngine::updatePositionsOfDebugRects()
{
	int i = 0;
	for(const auto& kinematicBody : mKinematicBodies) {
		auto pos = kinematicBody.getPosition();
		mCollisionDebugManager.mKinematicBodyCollisionDebugRects[i].setPosition(pos);
		++i;
	}
}

}