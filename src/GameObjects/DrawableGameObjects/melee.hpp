#pragma once

#include "GameObjects/GameObject.hpp"
#include <array>

namespace ph{

class GameData;
class Character;

class Swing {
public:
	Swing(const GameObject& opponentsNode, const sf::Vector2f direction, const sf::Vector2f position,
		const float damage, const float range, const float rotationRange);

	auto getPositionFromBeginning() const -> std::array<sf::Vertex, 2> { return { mStartPositionBeginning, mStartPositionEnd }; }

private:
	void setMeeleWeaponStartingPosition(const sf::Vector2f attackDirection);
	void handleHitCharacters();
	auto getCharacterWhoWasHit()->Character*;
	bool wasEnemyHit(Character&);
	void incrementRotation();

private:
	const GameObject& mEnemiesNode;
	const sf::Vector2f mDirection;
	const sf::Vector2f mStartPositionBeginning;
	const float mDamage;
	const float mRange;
	const float mRotationRange;

	float mRotation;
	sf::Vector2f mStartPositionEnd;
	std::array<sf::Vertex, 2> mHitArea;
};

class MeleeWeapon : public GameObject
{
public:
	MeleeWeapon(GameData* const, const float damage, const float range, const float rotationRange);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void attack(const sf::Vector2f attackDirection);

private:
	sf::Vector2f getRightHandPosition(const sf::Vector2f attackDirection);
	void initializeAttackGraphics(const Swing& swing);
	void updateHitGraphicsRotation();
	void resetAttackGraphics();

private:
	GameData* const mGameData;
	const float mDamage;
	const float mRange;
	const float mRotationRange;

	std::array<sf::Vertex, 2> mHitGraphics;
	float mGraphicsRotation;
	bool mShouldDrawSwing;
};

}