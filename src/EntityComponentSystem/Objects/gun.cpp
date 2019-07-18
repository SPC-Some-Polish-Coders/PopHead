#include "gun.hpp"
#include "gameData.hpp"

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
	:Object(gameData, "gun", LayerID::kinematicEntities)
	,mDamage(damage)
{
}

void Gun::shoot(const ShootDirection)
{
	mGameData->getSoundPlayer().playAmbientSound("sounds/barretaShot.wav");
}

void Gun::draw(sf::RenderTarget&, const sf::RenderStates) const
{
}

}