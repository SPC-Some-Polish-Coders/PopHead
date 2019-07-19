#include "gun.hpp"
#include "gameData.hpp"

namespace ph {

Bullet::Bullet(const Entity& enemiesNode, const sf::Vector2f direction, const sf::Vector2f startPosition,
               const float damage, const float range)
	:mEnemiesNode(enemiesNode)
	,mDirection(direction)
	,mDamage(damage)
	,mRange(range)
{
	characterWhoWasShot = getCharacterWhoWasShot();

	dealDamage();
}

auto Bullet::getCharacterWhoWasShot() -> Character*
{
	return nullptr;
}

void Bullet::makeSpriteOfShot()
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

void Gun::shoot(const ShotDirection shotDirection) const
{
	mGameData->getSoundPlayer().playAmbientSound("sounds/barretaShot.wav");
	auto& player = getParent();
	auto& root = player.getParent();
	auto& enemies = root.getChild("enemies");
	const sf::Vector2f shotDirectionVector = getShotDirectionVector(shotDirection);
	Bullet(enemies, shotDirectionVector, mPosition, 50, 200);
}

auto Gun::getShotDirectionVector(const ShotDirection shotDirection) const -> const sf::Vector2f
{
	switch(shotDirection)
	{
		case ShotDirection::east:
			return sf::Vector2f(1, 0);
		case ShotDirection::west:
			return sf::Vector2f(-1, 0);
		case ShotDirection::south:
			return sf::Vector2f(0, 1);
		case ShotDirection::north:
			return sf::Vector2f(0, -1);
		case ShotDirection::northEast:
			return sf::Vector2f(1, -1);
		case ShotDirection::northWest:
			return sf::Vector2f(-1, -1);
		case ShotDirection::southEast:
			return sf::Vector2f(1, 1);
		case ShotDirection::southWest:
			return sf::Vector2f(-1, 1);
	}
}

void Gun::draw(sf::RenderTarget&, const sf::RenderStates) const
{
}

}