#pragma once

#include "World/Entity/Objects/projectile.hpp"

namespace ph{

class AreaProjectile : public Projectile
{
public:
    AreaProjectile(GameData*, const std::string& name, float damage, float range, float radius);

private:
    void dealDamage(Object&) override;
    float lerpDamage();

private:
    float mRadius;
};

}
