#ifndef POPHEAD_PHYSICS_PHYSICSENGINE_H_
#define POPHEAD_PHYSICS_PHYSICSENGINE_H_

#include <SFML/Graphics.hpp>
#include <vector>

#include "collisionAxis.hpp"

namespace PopHead{
namespace Physics{

enum class CollisionSide
{
	left,
	right,
	top,
	down
};

class CollisionBody;

class PhysicsEngine
{
public:
    void addStaticBody(CollisionBody* staticBodyPtr);
    void addKinematicBody(CollisionBody* kinematicBodyPtr);
    void removeStaticBody(CollisionBody* staticBodyPtr);
    void removeKinematicBody(CollisionBody* kinematicBodyPtr);
    void clear() noexcept;

    void update(sf::Time delta);
private:
	void handleStaticCollisionsFor(CollisionBody* kinematicBody);
	bool isThereCollision(sf::FloatRect bodyA, sf::FloatRect bodyB);
	void setToContactPosition(CollisionBody* kinematicBody, CollisionBody* staticBody);
	auto getCollisionSide() -> CollisionSide;
	bool isBodyBetweenTopAndBottomAxisesOfAnotherBody(CollisionBody* bodyA, CollisionBody* bodyB);

private:
    std::vector<CollisionBody*> mStaticBodies;
    std::vector<CollisionBody*> mKinematicBodies;
};


}}

#endif // POPHEAD_PHYSICS_PHYSICSENGINE_H_
