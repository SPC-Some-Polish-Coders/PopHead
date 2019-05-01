#include "projectile.hpp"

using PopHead::World::Entity::Projectile;

Projectile::Projectile(PopHead::Base::GameData* gameData, const std::string& name, float damage, float range)
:Object(gameData, name, Renderer::LayerID::kinematicEntities)
,mDamage(damage)
,mRange(range)
,mDistanceTraveled(0)
{
}
