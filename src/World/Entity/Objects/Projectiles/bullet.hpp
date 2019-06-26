#pragma once

#include "World/Entity/Objects/projectile.hpp"

namespace ph{

class Bullet : public Projectile
{
public:
    Bullet(GameData*, const std::string& name, float damage, float range);

private:
    void dealDamage(Object&) override;
};

}
