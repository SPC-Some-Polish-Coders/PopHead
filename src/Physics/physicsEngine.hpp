#ifndef POPHEAD_PHYSICS_PHYSICSENGINE_H_
#define POPHEAD_PHYSICS_PHYSICSENGINE_H_

#include "collsionBody.hpp"
#include <SFML/System.hpp>

namespace PopHead{
namespace Physics{


class PhysicsEngine
{
public:
    void update(sf::Time delta);

    void addBody(CollisionBody* const);
    void removeBody(CollisionBody* const);
    void clear() noexcept;

private:
    std::vector<CollisionBody* const> staticBodies;
    std::vector<CollisionBody* const> kinematicBodies;
};


}}

#endif // POPHEAD_PHYSICS_PHYSICSENGINE_H_
