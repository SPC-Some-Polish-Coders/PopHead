#pragma once

#include "EntityComponentSystem/Objects/character.hpp"

namespace ph {

enum class ShotDirection 
{
	east, northEast, north, northWest, west, southWest, south, southEast
};

class Bullet
{
public:
	Bullet(const Entity& opponentsNode, const sf::Vector2f direction, const sf::Vector2f startPosition,
           const unsigned damage, const unsigned range);

private:
	auto getCharacterWhoWasShot() -> Character*;
	bool wasEnemyShot(Character&);
	bool isBulletStillInItsRange();
	void makeSpriteOfShot();

private:
	const sf::Vector2f mDirection;
	const sf::Vector2f mStartPosition;
	const Entity& mEnemiesNode;
	unsigned mTraveledDistance;
	const unsigned mRange;
	const unsigned mDamage;
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