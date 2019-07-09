#pragma once

#include "CollisionHandlers/staticCollisionHandler.hpp"
#include "CollisionHandlers/kinematicCollisionHandler.hpp"
#include "CollisionBody/collisionBody.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace ph{

class PhysicsEngine
{
public:
	PhysicsEngine();

    CollisionBody& createStaticBodyAndGetTheReference(const sf::FloatRect rect);
	CollisionBody& createKinematicBodyAndGetTheReference(const sf::FloatRect rect, float mass);

public:
    void clear() noexcept;

    void update(sf::Time delta);
private:
    void handleStaticCollisionsFor(CollisionBody& kinematicBody);
    void handleKinematicCollisionsFor(CollisionBody* kinematicBody);
    bool isThereCollision(sf::FloatRect bodyA, sf::FloatRect bodyB);

private:
    std::vector<std::unique_ptr<CollisionBody>> mStaticBodies;
    std::vector<std::unique_ptr<CollisionBody>> mKinematicBodies;

    StaticCollisionHandler mStaticCollisionHandler;
    KinematicCollisionHandler mKinematicCollisionHandler;
};

}
