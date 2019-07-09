#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "CollisionHandlers/staticCollisionHandler.hpp"
#include "CollisionHandlers/kinematicCollisionHandler.hpp"
#include "CollisionBody/collisionBody.hpp"

namespace ph{

class PhysicsEngine
{
public:
	PhysicsEngine();

    CollisionBody& createStaticBodyAndGetTheReference(const sf::FloatRect rect, const std::string& name);
	CollisionBody& createKinematicBodyAndGetTheReference(const sf::FloatRect rect, float mass, const std::string& name);

public:
    void clear() noexcept;

    void update(sf::Time delta);
private:
    void handleStaticCollisionsFor(CollisionBody& kinematicBody);
    void handleKinematicCollisionsFor(CollisionBody& kinematicBody);
    bool isThereCollision(sf::FloatRect bodyA, sf::FloatRect bodyB);

private:
    std::vector<CollisionBody> mStaticBodies;
    std::vector<CollisionBody> mKinematicBodies;

    StaticCollisionHandler mStaticCollisionHandler;
    KinematicCollisionHandler mKinematicCollisionHandler;
};

}
