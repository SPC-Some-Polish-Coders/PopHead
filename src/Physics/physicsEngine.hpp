#ifndef POPHEAD_PHYSICS_PHYSICSENGINE_H_
#define POPHEAD_PHYSICS_PHYSICSENGINE_H_

#include "collisionBody.hpp"
#include <SFML/System.hpp>

namespace PopHead{
namespace Physics{


class PhysicsEngine
{
public:
    void update(sf::Time delta);

    void addBody(CollisionBody*);
    void removeBody(CollisionBody*);
    void clear() noexcept;

private:
    std::vector<CollisionBody*> staticBodies;
    std::vector<CollisionBody*> kinematicBodies;
};


}}

#endif // POPHEAD_PHYSICS_PHYSICSENGINE_H_
