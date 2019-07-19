#include "gun.hpp"
#include "gameData.hpp"

namespace ph {

Bullet::Bullet(const Entity& opponentsNode, const sf::Vector2f direction, const sf::Vector2f startPosition,
               const float damage, const float range)
	:mOpponentsNode(opponentsNode)
	,mDirection(direction)
	,mDamage(damage)
	,mRange(range)
{
	const std::string nameOfCharacterWhoWasShot = getNameOfCharacterWhoWasShot();
	dealDamage(nameOfCharacterWhoWasShot);
}

auto Bullet::getNameOfCharacterWhoWasShot() -> const std::string
{
	return std::string();
}

void Bullet::dealDamage(const std::string nameOfObjectWhoWasShot)
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
	auto& opponents = root.getChild("opponents");
	const sf::Vector2f shotDirectionVector = getShotDirectionVector(shotDirection);
	Bullet(opponents, shotDirectionVector, mPosition, 50, 200);
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