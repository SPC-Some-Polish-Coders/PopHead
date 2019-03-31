#include "World/Entity/object.hpp"

namespace PopHead{
namespace World{
namespace Entity{


class Character : public Object
{
public:
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
