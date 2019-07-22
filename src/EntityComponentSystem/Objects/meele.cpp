#include "gameData.hpp"
#include "meele.hpp"

namespace ph {

Sword::Sword(GameData* const gameData, const float damage, const float range)
	:Object(gameData, "sword", LayerID::kinematicEntities)
	,mDamage(damage)
	,mRange(range)
	,mHitGraphic(sf::Triangles, 3)
{
}

void Sword::hit(const sf::Vector2f hitDirection)
{
	mGameData->getSoundPlayer().playAmbientSound("sounds/testSwordHit.wav");
	auto& player = getParent();
	auto& root = player.getParent();
	auto& enemies = root.getChild("enemy_container");
	initializeHit();
}

void Sword::initializeHit()
{

}

void Sword::resetHit()
{

}

void Sword::update(const sf::Time delta)
{
	if (mTimeFromLastHit.getElapsedTime().asSeconds() > 0.5f) 
		resetHit();
}

void Sword::draw(sf::RenderTarget& target, sf::RenderStates) const
{
	target.draw(mHitGraphic);
}

}