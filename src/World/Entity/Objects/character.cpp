#include "character.hpp"

#include <iostream>

using PopHead::World::Entity::Character;

Character::Character(Base::GameData* gameData, std::string name,
                     unsigned int HP, unsigned int maxHP, unsigned int movementSpeed)
:Object(gameData, name, Renderer::LayerID::kinematicEntities)
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

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);

    std::cout<<"draw()"<<std::endl;
}

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
