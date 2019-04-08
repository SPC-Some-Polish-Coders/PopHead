#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTER_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTER_H_

#include "World/Entity/object.hpp"
#include "characterMotion.hpp"
#include "World/Animation/animation.hpp"

namespace PopHead{
namespace World{
namespace Entity{


class Character : public Object
{
public:
    Character(Base::GameData*, std::string name,
              unsigned int mMovementSpeed = 50, unsigned int HP = 100, unsigned int maxHP = 100);

    void atack();
    void onCollision(Object&) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    unsigned int getMaxHP() const;
    unsigned int getHP() const;
    sf::Sprite& getSprite();
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
};


}}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTER_H_
