#ifndef POPHEAD_PHYSICS_PHYSICSENGINE_H_
#define POPHEAD_PHYSICS_PHYSICSENGINE_H_

#include <SFML/Graphics.hpp>
#include <vector>

#include "collisionAxis.hpp"

namespace PopHead{
namespace Physics{


class CollisionBody;

class PhysicsEngine
{
public:
    void update(sf::Time delta);

    void addStaticBody(CollisionBody* staticBodyPtr);
    void addKinematicBody(CollisionBody* kinematicBodyPtr);
    void removeStaticBody(CollisionBody* staticBodyPtr);
    void removeKinematicBody(CollisionBody* kinematicBodyPtr);
    void clear() noexcept;

private:
	void handleStaticCollisionsForThisKinematicBody(CollisionBody* kinematicBody) const;
	CollisionAxis getAxisOfCollision(CollisionBody* kinematicBody, CollisionBody* staticBody);
	bool isThereCollision(sf::FloatRect bodyA, sf::FloatRect bodyB);

private:
    std::vector<CollisionBody*> mStaticBodies;
    std::vector<CollisionBody*> mKinematicBodies;
};


}}

#endif // POPHEAD_PHYSICS_PHYSICSENGINE_H_
