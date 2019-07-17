#include "gun.hpp"

namespace ph {

Bullet::Bullet(const float damage, const float range)
	:mDamage(damage)
	,mRange(range)
{
}

void Bullet::dealDamage()
{
}

Gun::Gun(GameData* const gameData, const float damage)
	:Object(gameData, "bullet", LayerID::kinematicEntities)
	,mDamage(damage)
{
}

void Gun::shoot(const ShootDirection)
{
}

}