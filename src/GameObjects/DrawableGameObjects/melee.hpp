#pragma once

#include "GameObjects/DrawableGameObject.hpp"
#include "GameObjects/GameObjectContainers/enemyContainer.hpp"
#include <array>

namespace ph{

class Character;

class Swing {
public:
	Swing(const GameObject& opponentsNode, const sf::Vector2f direction, const sf::Vector2f position,
		const unsigned damage, const unsigned range, const unsigned rotationRange);

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

class MeleeWeapon : public DrawableGameObject
{
public:
	MeleeWeapon(GameData* const, const float damage, const float range, const float rotationRange);

	void update(const sf::Time delta) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void attack(const sf::Vector2f attackDirection);

private:
	void setMeleeWeaponPositionToRightHand(const sf::Vector2f attackDirection);
	void initializeAttackGraphics(const Swing& swing);
	void updateHitGraphicsRotation();
	void resetAttackGraphics();
	auto getEnemies()->GameObject&;

private:
	const float mDamage;
	const float mRange;
	const float mRotationRange;

	std::array<sf::Vertex, 2> mHitGraphics;
	float mGraphicsRotation;
	bool mShouldDrawSwing;
};

}