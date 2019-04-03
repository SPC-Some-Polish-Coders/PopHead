#include "character.hpp"

using PopHead::World::Entity::Character;

Character::Character(EntityType type, Base::GameData* gameData, std::string name, Renderer::LayerID layerID,
                     unsigned int HP, unsigned int maxHP, unsigned int movementSpeed)
:Object(type, gameData, name, layerID)
,mHP(HP)
,mMaxHP(maxHP)
,mMovementSpeed(movementSpeed)
{
}

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
