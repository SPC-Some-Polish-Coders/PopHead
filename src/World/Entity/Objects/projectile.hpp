#pragma once

#include "World/Entity/object.hpp"

namespace ph{

class Projectile : public Object
{
public:
    Projectile(GameData*, const std::string& name, float damage, float range);
    virtual void dealDamage(Object&) = 0;

private:
    float mDamage;
    const float mRange;
    float mDistanceTraveled;
};

}
