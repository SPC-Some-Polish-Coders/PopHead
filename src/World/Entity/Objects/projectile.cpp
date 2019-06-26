#include "projectile.hpp"

namespace ph {

Projectile::Projectile(GameData* gameData, const std::string& name, float damage, float range)
	:Object(gameData, name, LayerID::kinematicEntities)
	,mDamage(damage)
	,mRange(range)
	,mDistanceTraveled(0)
{
}

}