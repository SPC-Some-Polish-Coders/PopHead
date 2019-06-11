#include "areaProjectile.hpp"

ph::AreaProjectile::AreaProjectile(GameData* gameData, const std::string& name, float damage, float range, float radius)
	:Projectile(gameData, name, damage, range)
	,mRadius(radius)
{
}

void ph::AreaProjectile::dealDamage(Object&)
{

}

float ph::AreaProjectile::lerpDamage()
{
	return 0.f;
}
