#pragma once

#include "CollisionHandlers/staticCollisionHandler.hpp"
#include "CollisionHandlers/kinematicCollisionHandler.hpp"
#include "CollisionBody/collisionBody.hpp"
#include "CollisionDebug/collisionDebugManager.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <set>
#include <functional>

namespace ph{

class PhysicsEngine
{
public:
	PhysicsEngine();

    const CollisionBody& createStaticBodyAndGetTheReference(const sf::FloatRect rect);
	CollisionBody& createKinematicBodyAndGetTheReference(const sf::FloatRect rect, const float mass);
	void removeStaticBody(const CollisionBody&);
	void removeKinematicBody(const CollisionBody&);
    void clear() noexcept;

	const size_t howManyStaticBodiesAreThere() const { return mStaticBodies.size(); }
	const size_t howManyKinematicBodiesAreThere() const { return mKinematicBodies.size(); }
	auto getCollisionDebugManager() const -> const CollisionDebugManager& { return mCollisionDebugManager; }

    void update(sf::Time delta);
private:
    void handleStaticCollisionsFor(CollisionBody& kinematicBody);
    void handleKinematicCollisionsFor(CollisionBody& kinematicBody);
    bool isThereCollision(const CollisionBody& a, const CollisionBody& b) const;
	void updatePositionsOfDebugRects();

private:
    //std::vector<std::unique_ptr<CollisionBody>> mStaticBodiesOld;
	std::set<CollisionBody, std::function<bool(const CollisionBody&, const CollisionBody&)>> mStaticBodies;
    std::list<CollisionBody> mKinematicBodies;
	CollisionDebugManager mCollisionDebugManager;
    StaticCollisionHandler mStaticCollisionHandler;
    KinematicCollisionHandler mKinematicCollisionHandler;
};

}
