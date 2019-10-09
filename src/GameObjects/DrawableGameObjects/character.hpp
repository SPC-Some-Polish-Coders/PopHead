#pragma once

#include "GameObjects/gameObject.hpp"
#include "GameObjects/DrawableGameObjects/item.hpp"
#include "Utilities/animation.hpp"
#include "Renderer/renderer.hpp"
#include <memory>

namespace ph {

class CollisionBody;
class GameData;

class Character : public GameObject
{
public:
	Character(GameData*, std::string name, const Texture&, Animation animation = Animation(),
		unsigned int mMovementSpeed = 50, int HP = 100, unsigned int maxHP = 100,
		sf::FloatRect posAndSize = sf::FloatRect(0, 0, 0, 0), float mass = 50, bool isAttackable = false);

	void drawCurrent(sf::Transform) override;

	void dropItems();

	void setPosition(sf::Vector2f);
	
	void setHp(unsigned hp);
	void addHp(int hp);
	void setMaxHp(unsigned int maxHp);
	int getHp() const;
	bool isDead() const;

	void move(sf::Vector2f);
	void takeDamage(const unsigned damage);
	void drawBlood();
	void setAnimationState(const std::string& stateName);
	auto getSpriteCenter() -> sf::Vector2f;
	sf::FloatRect getGlobalBounds() const override;
	sf::FloatRect getTextureBounds() const;
	auto getAnimation() -> Animation& { return mAnimation; }
	bool isAttackable() const { return mIsAttackable; }

	void pushCharacter(const sf::Vector2f& pushVector);

private:
	void fixHp();

protected:
	Sprite mSprite;
	Animation mAnimation;
	sf::Clock mTimeSinceLastTakenDamage;
	GameData* const mGameData;
	CollisionBody& mCollisionBody;
	int mHp;
	unsigned mMaxHp;
	unsigned mMovementSpeed;
	bool mIsAttackable;

	inline static bool mIsInAttackingMode = false;
	inline static int mSerialNumber = 0;
	inline static Shader* mShader = nullptr;
};

}
