#pragma once

#include "GameObjects/DrawableGameObject.hpp"
#include <array>

namespace ph{

class Character;

class Swoosh {
public:
	Swoosh(const GameObject& opponentsNode, const sf::Vector2f direction, const sf::Vector2f position,
		const unsigned damage, const unsigned range);

	auto getHitArea() const -> std::array<sf::Vertex, 4> { return mHitArea; }

private:
	void setMeeleWeaponHitArea(const sf::Vector2f attackDirection, const sf::Vector2f startPosition);
	auto getCharacterWhoWasShot()->Character*;
	bool wasEnemyHit(Character&);

private:
	const GameObject& mEnemiesNode;
	const sf::Vector2f mDirection;
	const float mDamage;
	const float mRange;
	std::array<sf::Vertex, 4> mHitArea;
};

class MeleeWeapon : public DrawableGameObject
{
public:
	MeleeWeapon(GameData* const, const float damage, const float range);

	void update(const sf::Time delta) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void attack(const sf::Vector2f attackDirection);

private:
	void setMeleeWeaponPositionToRightHand(const sf::Vector2f attackDirection);
	void initializeAttackGraphics(const Swoosh& swoosh);
	void resetAttackGraphics();

private:
	std::array<sf::Vertex, 4> mHitGraphics;
	const float mDamage;
	const float mRange;
	sf::Clock mTimeFromLastAttack;
};

}