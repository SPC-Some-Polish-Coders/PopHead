#include "bullet.hpp"

using ph::World::Entity::Bullet;

Bullet::Bullet(ph::Base::GameData* gameData, const std::string& name, float damage, float range)
:Projectile(gameData, name, damage, range)
{
}

void Bullet::dealDamage(Object&)
{

}
