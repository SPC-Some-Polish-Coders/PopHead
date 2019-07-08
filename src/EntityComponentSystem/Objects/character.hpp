#pragma once

#include "EntityComponentSystem/object.hpp"
#include "EntityComponentSystem/Objects/Characters/Motion/characterMotion.hpp"
#include "Utilities/animation.hpp"

namespace ph {

class CollisionBody;

class Character : public Object
{
public:
	Character(GameData*, std::string name, Animation animation = Animation(),
		unsigned int mMovementSpeed = 50, unsigned int HP = 100, unsigned int maxHP = 100,
		sf::FloatRect posAndSize = sf::FloatRect(0, 0, 0, 0), float mass = 50);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setPosition(sf::Vector2f, bool recursive = true) override;
	void move(sf::Vector2f, bool recursive = true) override;
	void setScale(sf::Vector2f, bool recursive = true) override;
	void setRotation(float angle, bool recursive = true) override;
	void rotate(float angle, bool recursive = true) override;

	auto getSprite() -> sf::Sprite& { return mSprite; }

protected:
	unsigned int mHP;
	unsigned int mMaxHP;
	unsigned int mMovementSpeed;
	CharacterMotion mMotion;
	sf::Sprite mSprite;
	Animation mAnimation;
	CollisionBody& mCollisionBody;
};

}
