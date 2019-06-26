#include "areaProjectile.hpp"

namespace ph {

AreaProjectile::AreaProjectile(GameData* gameData, const std::string& name, float damage, float range, float radius)
	:Projectile(gameData, name, damage, range)
	,mRadius(radius)
{
}

void AreaProjectile::dealDamage(Object&)
{

}

float AreaProjectile::lerpDamage()
{
	return 0.f;
}

}