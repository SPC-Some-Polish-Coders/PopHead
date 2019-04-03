#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTER_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTER_H_

#include "World/Entity/object.hpp"

namespace PopHead{
namespace World{
namespace Entity{


class Character : public Object
{
public:
    Character(EntityType, Base::GameData*, std::string name, Renderer::LayerID,
              unsigned int HP = 100, unsigned int maxHP = 100, unsigned int mMovementSpeed = 10);

    void atack();
    ///void onColision(Object&) override;
    unsigned int getMaxHP() const;
    unsigned int getHP() const;
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
};


}}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTER_H_
