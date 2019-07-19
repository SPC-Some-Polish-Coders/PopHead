#pragma once

#include "EntityComponentSystem/object.hpp"

namespace ph {

enum class ShotDirection 
{
	east, northEast, north, northWest, west, southWest, south, southEast
};

class Bullet
{
public:
	Bullet(const Entity& opponentsNode, const sf::Vector2f direction, const sf::Vector2f startPosition,
           const float damage, const float range);

private:
	const std::string getNameOfCharacterWhoWasShot();
	void dealDamage(const std::string nameOfObjectWhoWasShot);

private:
	const Entity& mEnemiesNode;
	const sf::Vector2f mDirection;
	const sf::Vector2f mStartPosition;
	const float mDamage;
	const float mRange;
};

class Gun : public Object
{
public:
	Gun(GameData* const, const float damage);

	void shoot(const ShotDirection) const;
	auto getShotDirectionVector(const ShotDirection) const -> const sf::Vector2f;
	void draw(sf::RenderTarget&, const sf::RenderStates) const override;

private:
	const float mDamage;
};

}