#pragma once

#include "GameObjects/gameObject.hpp"
#include "Renderer/renderer.hpp"
#include "Utilities/rect.hpp"
#include <array>

namespace ph {

class SoundPlayer;
class Character;

class Bullet
{
public:
	Bullet(const GameObject& opponentsNode, const sf::Vector2f direction, const sf::Vector2f startPosition,
           const unsigned damage, const unsigned range);

	auto getStartPosition() const -> const sf::Vector2f { return mStartPosition; }
	auto getCurrentPosition() const -> const sf::Vector2f;

private:
	auto getCharacterWhoWasShot() -> Character*;
	auto getCharactersInShotArea() -> std::vector<Character*>;
	FloatRect getShotArea();
	sf::Vector2f getShotAreaTopLeftCorner() const;
	sf::Vector2f getShotAreaSize() const;
	auto getFirstCharacterOnShotLine(std::vector<Character*> charactersInShotArea) -> Character*;
	bool wasCharacterShot(Character*, const sf::Vector2f currentBulletPosition);
	bool isBulletStillInItsRange();

private:
	const sf::Vector2f mDirection;
	const sf::Vector2f mStartPosition;
	const GameObject& mNodeWithAttackableObjects;
	unsigned mTraveledDistance;
	const unsigned mRange;
	const unsigned mDamage;
};

class Gun : public GameObject
{
public:
	Gun(SoundPlayer&, const Texture&, const float damage);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::Transform) override;
	void shoot();

	void setCurrentPlayerDirection(const sf::Vector2f cpd) { mCurrentPlayerDirection = cpd; }

private:
	void initializeShotGraphics(const Bullet&, const sf::Vector2f rightHandPosition);
	void updateGunTextureRect(const int offsetX = 0);
	void updateGunSpriteFlipping();
	void updateGunSpritePosition();
	sf::Vector2f getRightHandPosition();

private:
	Sprite mGunSprite;
	std::array<sf::Vertex, 2> mShotGraphics;
	sf::Clock mTimeFromTrigerPull;
	sf::Vector2f mCurrentPlayerDirection;
	SoundPlayer& mSoundPlayer;
	const float mDamage;
	bool mShouldDisplayShotGraphics;
	bool mShouldDisplayGunSprite;
};

}
