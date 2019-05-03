#ifndef POPHEAD_PHYSICS_COLLISIONSDEBUG_H_
#define POPHEAD_PHYSICS_COLLISIONSDEBUG_H_

#include <vector>
#include "World/Entity/Objects/collisionDebugRect.hpp"

namespace PopHead{
namespace Physics{


class CollisionBody;

class CollisionDebug
{
public:
    void createCollisionDebugRects(const std::vector<CollisionBody*>& kinematicBodies,
                                   const std::vector<CollisionBody*>& staticBodies);
    void addCollisonDebugRect(const CollisionBody*);

    void clearCollisonDebugRects();

private:
    std::vector<PopHead::World::Entity::CollisionDebugRect> mCollisionDebugRects;
};


}}

#endif // POPHEAD_PHYSICS_COLLISIONSDEBUG_H_
