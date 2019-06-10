#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTER_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTER_H_

#include "World/Entity/object.hpp"
#include "World/Entity/Objects/Characters/Motion/characterMotion.hpp"
#include "World/Animation/animation.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"

namespace PopHead {
namespace World {
namespace Entity {


class Character : public Object
{
public:
	Character(Base::GameData*, std::string name, Animation animation = Animation(),
		unsigned int mMovementSpeed = 50, unsigned int HP = 100, unsigned int maxHP = 100,
		sf::FloatRect posAndSize = sf::FloatRect(0, 0, 0, 0), float mass = 50);

	void atack();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setPosition(sf::Vector2f, bool recursive = true) override;
	void move(sf::Vector2f, bool recursive = true) override;
	void setScale(sf::Vector2f, bool recursive = true) override;
	void setRotation(float angle, bool recursive = true) override;
	void rotate(float angle, bool recursive = true) override;

	auto getSprite() -> sf::Sprite& { return mSprite; }
	///auto getEquipment() const -> const Equipment&;
	///auto getCurrentWeapon() const -> const Weapon&;
	///auto getExperienceManager() const -> const ExperienceManager&;

protected:
	///Equipment mEquipment;
	///Weapon* const mCurrentWeapon;
	///ExperienceManager mExperienceManager;
	unsigned int mHP;
	unsigned int mMaxHP;
	unsigned int mMovementSpeed;
	CharacterMotion mMotion;
	sf::Sprite mSprite;
	Animation mAnimation;
	Physics::CollisionBody mCollisionBody;
};


}}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTER_H_
