#ifndef POPHEAD_PHYSICS_COLLISIONSDEBUG_H_
#define POPHEAD_PHYSICS_COLLISIONSDEBUG_H_

#include <vector>
#include "World/Entity/Objects/collisionDebugRect.hpp"

namespace PopHead{
namespace Physics{


class CollisionDebug
{
public:
    collisionDebug();

private:
    std::vector<PopHead::World::Entity::CollisionDebugRect> mCollisionDebugRects;
};


}}

#endif // POPHEAD_PHYSICS_COLLISIONSDEBUG_H_
