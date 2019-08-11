#pragma once

#include "GameObjects/drawableGameObject.hpp"
#include "Utilities/animation.hpp"

namespace ph {

class CollisionBody;
class GameData;

class Character : public DrawableGameObject
{
public:
	Character(GameData*, std::string name, Animation animation = Animation(),
		unsigned int mMovementSpeed = 50, int HP = 100, unsigned int maxHP = 100,
		sf::FloatRect posAndSize = sf::FloatRect(0, 0, 0, 0), float mass = 50);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setPosition(sf::Vector2f, bool recursive = true) override;
	void setHp(int hp) { mHp = hp; }
	void setMaxHp(unsigned int maxHp) { mMaxHp = maxHp; }
	void move(sf::Vector2f, bool recursive = true) override;
	void setScale(sf::Vector2f, bool recursive = true) override;
	void setRotation(float angle, bool recursive = true) override;
	void rotate(float angle, bool recursive = true) override;
	void takeDamage(const unsigned damage);
	void drawBlood();

	auto getSprite() -> sf::Sprite& { return mSprite; }
	auto getSpriteCenter() -> sf::Vector2f;

protected:
	sf::Sprite mSprite;
	Animation mAnimation;
	sf::Clock mTimeSinceLastTakenDamage;
	GameData* const mGameData;
	CollisionBody& mCollisionBody;
	int mHp;
	unsigned mMaxHp;
	unsigned mMovementSpeed;
	bool mIsDead;
};

}
