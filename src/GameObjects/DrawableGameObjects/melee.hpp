#pragma once

#include "GameObjects/GameObject.hpp"
#include <array>

namespace ph{

class GameData;
class Character;
class CollisionBody;

class Swing 
{
public:
	Swing(const GameObject& nodeWithAttackableObjects, const sf::Vector2f position,
		const float damage, const float range, const float rotationRange, float attackAngle);

private:
	void handleHitCharacters();
	sf::Vector2f nearestPointOfCharacter(const Character& character) const;
	float angleOfPointToStart(sf::Vector2f point) const;
	auto getAttackableCharactersInHitArea() const -> std::vector<Character*>;
	bool isAngleInAttackRange(float angle) const;
	
	static float getFixedAngle(float angle);

private:
	const GameObject& mNodeWithAttackableObjects;
	const sf::Vector2f mStartPosition;
	const float mDamage;
	const float mRange;
	const float mRotationRange;
	float mRotation;
	float mAttackAngle;
};

class MeleeWeapon : public GameObject
{
public:
	MeleeWeapon(GameData* const, const float damage, const float range, const float rotationRange);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::Transform) override;
	void attack(const sf::Vector2f attackDirection, float attackRotation);

private:
	float getStartAttackRotation(const sf::Vector2f attackDirection) const;
	sf::Vector2f getRightHandLocalPosition(const sf::Vector2f attackDirection);

private:
	sf::Sprite mSprite;
	GameData* const mGameData;
	const float mDamage;
	const float mRange;
	const float mRotationRange;
	float mRotationFromStart;
	bool mShouldBeDrawn;
	CollisionBody* mEndOfMelle = nullptr;
};

}
