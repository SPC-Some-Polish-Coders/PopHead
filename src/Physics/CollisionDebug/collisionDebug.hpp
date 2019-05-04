#ifndef POPHEAD_PHYSICS_COLLISIONSDEBUG_H_
#define POPHEAD_PHYSICS_COLLISIONSDEBUG_H_

#include <vector>
#include "collisionDebugRect.hpp"
#include "Physics/bodyType.hpp"

namespace PopHead{
namespace Physics{


class CollisionBody;

class CollisionDebug
{
public:
    void createFrom(const std::vector<CollisionBody*>& kinematicBodies,
                    const std::vector<CollisionBody*>& staticBodies);
    void createFrom(const CollisionBody* collisionBody, BodyType type);

    void clear();

private:
    std::vector<PopHead::World::Entity::CollisionDebugRect> mCollisionDebugRects;
};


}}

#endif // POPHEAD_PHYSICS_COLLISIONSDEBUG_H_
