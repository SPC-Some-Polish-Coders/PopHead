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
	Swing(const GameObject& nodeWithAttackableObjects, const sf::Vector2f direction, const sf::Vector2f position,
		const float damage, const float range, const float rotationRange, float attackAngle);

private:
	void setMeeleWeaponStartingPosition(const sf::Vector2f& attackDirection);
	void handleHitCharacters(const sf::Vector2f& attackDirection);
	sf::Vector2f nearestPointOfCharacter(const Character& character) const;
	float angleOfPointToStart(sf::Vector2f point) const;
	auto getAttackableCharactersInHitArea() const -> std::vector<Character*>;
	bool wasCharacterHit(Character*);
	void incrementRotation();
	bool isAngleInAttackRange(float angle) const;
	
	static float getFixedAngle(float angle);

private:
	const GameObject& mNodeWithAttackableObjects;
	const sf::Vector2f mDirection;
	const sf::Vector2f mStartPosition;
	const float mDamage;
	const float mRange;
	const float mRotationRange;
	float mRotation;
	float mAttackAngle;
	std::array<sf::Vertex, 2> mHitArea;
};

class MeleeWeapon : public GameObject
{
public:
	MeleeWeapon(GameData* const, const float damage, const float range, const float rotationRange);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
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
