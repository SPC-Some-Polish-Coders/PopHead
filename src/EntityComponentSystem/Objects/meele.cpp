#include "gameData.hpp"
#include "meele.hpp"

namespace ph {

Swoosh::Swoosh(const Entity& opponentsNode, const sf::Vector2f direction, const unsigned damage, 
	const unsigned range)
	:mEnemiesNode(opponentsNode)
	,mDirection(direction)
	,mDamage(damage)
	,mRange(range)
{
}


MeeleWeapon::MeeleWeapon(GameData* const gameData, const float damage, const float range)
	:Object(gameData, "sword", LayerID::kinematicEntities)
	,mDamage(damage)
	,mRange(range)
{
}

void MeeleWeapon::attack(const sf::Vector2f hitDirection)
{
	mGameData->getSoundPlayer().playAmbientSound("sounds/swordAttack.wav");
	setMeeleWeaponPositionToRightHand(hitDirection);
	initializeAttackGraphics();
}

void MeeleWeapon::setMeeleWeaponPositionToRightHand(const sf::Vector2f attackDirection)
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

void MeeleWeapon::initializeAttackGraphics()
{

}

void MeeleWeapon::resetAttack()
{

}

void MeeleWeapon::update(const sf::Time delta)
{
}

void MeeleWeapon::draw(sf::RenderTarget& target, sf::RenderStates) const
{

}

}