#include "character.hpp"

using PopHead::World::Entity::Character;

void Character::atack()
{

}

/*void Character::onColision(Object&)
{

}*/

unsigned int Character::getMaxHP() const
{
    return mMaxHP;
}

unsigned int Character::getHP() const
{
    return mHP;
}

/*auto Character::getEquipment() const -> const Equipment&
{

}*/

/*auto Character::getCurrentWeapon() const -> const Weapon&
{

}*/

/*auto Character::getExperienceManager() const -> const ExperienceManager&
{

}*/
