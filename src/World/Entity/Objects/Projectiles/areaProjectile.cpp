#include "areaProjectile.hpp"

using PopHead::World::Entity::AreaProjectile;

AreaProjectile::AreaProjectile(Base::GameData* gameData, const std::string& name, float damage, float range, float radius)
:Projectile(gameData, name, damage, range)
,mRadius(radius)
{
}

void AreaProjectile::dealDamage(Object&)
{

}

float AreaProjectile::lerpDamage()
{

}
