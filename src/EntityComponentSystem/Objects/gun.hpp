#pragma once

#include "EntityComponentSystem/Objects/character.hpp"
#include <array>

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

	auto getStartPosition() const -> const sf::Vector2f { return mStartPosition; }
	auto getCurrentPosition() const -> const sf::Vector2f { return mStartPosition + (mDirection * static_cast<float>(mTraveledDistance)); }

private:
	auto getCharacterWhoWasShot() -> Character*;
	bool wasEnemyShot(Character&);
	bool isBulletStillInItsRange();

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

	void shoot(const ShotDirection);
	auto getShotDirectionVector(const ShotDirection) const -> const sf::Vector2f;
	void initializeShotGraphics(const Bullet&);
	void draw(sf::RenderTarget&, const sf::RenderStates) const override;

private:
	const float mDamage;
	std::array<sf::Vertex, 2> mShotGraphics;
};

}