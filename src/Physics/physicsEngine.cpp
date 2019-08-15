#include "physicsEngine.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"

namespace ph {

PhysicsEngine::PhysicsEngine()
	: mStaticBodies(std::function<bool(const CollisionBody&, const CollisionBody&)>(
		[](const CollisionBody& b1, const CollisionBody& b2) -> bool {
			auto pos1 = b1.getPosition();
			auto pos2 = b2.getPosition();

			if (pos1.x < pos2.x)
				return true;
			else if (pos1.x == pos2.x && pos1.y < pos2.y)
				return true;
			return false;
		}
		))
{
}

const CollisionBody& PhysicsEngine::createStaticBodyAndGetTheReference(const sf::FloatRect rect)
{
	auto iter = mStaticBodies.emplace(rect, 0.f);
	//mCollisionDebugManager.addStaticBodyCollisionDebugRect(*iter.first);
	//return *iter.first;
	mCollisionDebugManager.addStaticBodyCollisionDebugRect(*iter);
	return *iter;
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
	mStaticBodies.erase(CollisionBody(bodyToDelete));
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
	float marginOfCollisionsChecking = 48.f;
	auto rect = kinematicBody.getRect();
	rect.left -= marginOfCollisionsChecking;
	rect.top -= marginOfCollisionsChecking;

	auto iter = mStaticBodies.lower_bound(CollisionBody(rect, 0.f));
	auto end = mStaticBodies.end();

	auto maxPosition = kinematicBody.getPosition().x + marginOfCollisionsChecking;

	while (iter != end)
	{
		if (iter->getPosition().x > maxPosition)
			break;

		if (isThereCollision(kinematicBody, *iter))
			mStaticCollisionHandler(kinematicBody, *iter);

		++iter;
	}
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