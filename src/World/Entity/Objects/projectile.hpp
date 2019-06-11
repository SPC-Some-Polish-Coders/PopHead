#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_PROJECTILE_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_PROJECTILE_H_

#include "World/Entity/object.hpp"

namespace ph{
namespace World{
namespace Entity{


class Projectile : public Object
{
public:
    Projectile(Base::GameData*, const std::string& name, float damage, float range);
    virtual void dealDamage(Object&) = 0;

private:
    float mDamage;
    const float mRange;
    float mDistanceTraveled;
};


}}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_PROJECTILE_H_
