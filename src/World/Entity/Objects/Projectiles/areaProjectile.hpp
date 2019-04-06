#ifndef POPHEAD_WORLD_ENTITY_PROJECTILES_AREAPROJECTILE_PLAYER_H_
#define POPHEAD_WORLD_ENTITY_PROJECTILES_AREAPROJECTILE_PLAYER_H_

#include "World/Entity/Objects/projectile.hpp"

namespace PopHead{
namespace World{
namespace Entity{


class AreaProjectile : public Projectile
{
public:
    AreaProjectile(Base::GameData*, const std::string& name, float damage, float range, float radius);

private:
    void dealDamage(Object&) override;
    float lerpDamage();

private:
    float mRadius;
};


}}}

#endif // POPHEAD_WORLD_ENTITY_PROJECTILES_AREAPROJECTILE_PLAYER_H_
