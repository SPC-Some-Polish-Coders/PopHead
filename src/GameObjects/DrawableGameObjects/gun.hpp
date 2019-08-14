#pragma once

#include "GameObjects/DrawableGameObjects/character.hpp"
#include <array>

namespace ph {

class GameData;

class Bullet
{
public:
	Bullet(const GameObject& opponentsNode, const sf::Vector2f direction, const sf::Vector2f startPosition,
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
	const GameObject& mEnemiesNode;
	unsigned mTraveledDistance;
	const unsigned mRange;
	const unsigned mDamage;
};

class Gun : public DrawableGameObject
{
public:
	Gun(GameData* const, const float damage);

	void updateCurrent(const sf::Time delta) override;
	void draw(sf::RenderTarget&, const sf::RenderStates) const override;
	void shoot(const sf::Vector2f shotDirection);

private:
	void setGunPositionToRightHand(const sf::Vector2f shotDirection);
	void initializeShotGraphics(const Bullet&);
	void resetShotGraphics();

private:
	std::array<sf::Vertex, 2> mShotGraphics;
	sf::Clock mTimeFromTrigerPull;
	GameData* const mGameData;
	const float mDamage;
};

}