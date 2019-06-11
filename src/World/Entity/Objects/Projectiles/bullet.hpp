#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_PROJECTILES_BULLET_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_PROJECTILES_BULLET_H_

#include "World/Entity/Objects/projectile.hpp"

namespace ph{
namespace World{
namespace Entity{


class Bullet : public Projectile
{
public:
    Bullet(Base::GameData*, const std::string& name, float damage, float range);

private:
    void dealDamage(Object&) override;
};


}}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_PROJECTILES_BULLET_H_
