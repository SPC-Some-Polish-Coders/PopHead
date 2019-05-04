#ifndef POPHEAD_PHYSICS_COLLISIONSDEBUG_H_
#define POPHEAD_PHYSICS_COLLISIONSDEBUG_H_

#include <vector>
#include "collisionDebugRect.hpp"

namespace PopHead{
namespace Physics{


class CollisionBody;
enum class BodyType;

class CollisionDebug
{
public:
    void createFrom(std::vector<CollisionBody*>& kinematicBodies, std::vector<CollisionBody*>& staticBodies);
    void createFrom(const CollisionBody* collisionBody, BodyType type);

    void clear();

private:
    std::vector<PopHead::World::Entity::CollisionDebugRect> mCollisionDebugRects;
};


}}

#endif // POPHEAD_PHYSICS_COLLISIONSDEBUG_H_
