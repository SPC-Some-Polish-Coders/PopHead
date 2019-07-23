#include "gameData.hpp"
#include "melee.hpp"

namespace ph {

Swoosh::Swoosh(const Entity& opponentsNode, const sf::Vector2f direction, const unsigned damage, 
	const unsigned range)
	:mEnemiesNode(opponentsNode)
	,mDirection(direction)
	,mDamage(damage)
	,mRange(range)
{
}


MeleeWeapon::MeleeWeapon(GameData* const gameData, const float damage, const float range)
	:Object(gameData, "sword", LayerID::kinematicEntities)
	,mDamage(damage)
	,mRange(range)
{
}

void MeleeWeapon::attack(const sf::Vector2f hitDirection)
{
	mGameData->getSoundPlayer().playAmbientSound("sounds/swordAttack.wav");
	setMeleeWeaponPositionToRightHand(hitDirection);
	initializeAttackGraphics();
}

void MeleeWeapon::setMeleeWeaponPositionToRightHand(const sf::Vector2f attackDirection)
{
	if (attackDirection == sf::Vector2f(1, 0))
		mPosition += {20, 20};
	else if (attackDirection == sf::Vector2f(-1, 0))
		mPosition += {5, 15};
	else if (attackDirection == sf::Vector2f(0, 1))
		mPosition += {3, 20};
	else if (attackDirection == sf::Vector2f(0, -1))
		mPosition += {15, 15};
	else if (attackDirection == sf::Vector2f(0.7f, -0.7f))
		mPosition += {20, 3};
	else if (attackDirection == sf::Vector2f(-0.7f, -0.7f))
		mPosition += {3, 3};
	else if (attackDirection == sf::Vector2f(0.7f, 0.7f))
		mPosition += {10, 20};
	else if (attackDirection == sf::Vector2f(-0.7f, 0.7f))
		mPosition += {0, 10};
	else
		PH_EXCEPTION("Direction vector like this shouldn't exist.");
}

void MeleeWeapon::initializeAttackGraphics()
{

}

void MeleeWeapon::resetAttack()
{

}

void MeleeWeapon::update(const sf::Time delta)
{
}

void MeleeWeapon::draw(sf::RenderTarget& target, sf::RenderStates) const
{

}

}