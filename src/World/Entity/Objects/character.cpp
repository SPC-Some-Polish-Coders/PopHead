#include "character.hpp"

#include "Physics/bodyType.hpp"

#include <iostream>

using PopHead::World::Entity::Character;

Character::Character(PopHead::Base::GameData* gameData, std::string name, PopHead::World::Animation animation,
                     unsigned int movementSpeed, unsigned int HP, unsigned int maxHP, sf::FloatRect posAndSize, float mass)
:Object(gameData, name, Renderer::LayerID::kinematicEntities)
,mHP(HP)
,mMaxHP(maxHP)
,mMovementSpeed(movementSpeed)
,mMotion()
,mAnimation(animation)
,mCollisionBody(posAndSize, mass, Physics::BodyType::kinematicBody, this, gameData)
{
}

void Character::atack()
{

}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void Character::setPosition(sf::Vector2f position)
{
    mSprite.setPosition(position);
    Object::setPosition(position);
}

void Character::move(sf::Vector2f offset)
{
    mSprite.move(offset);
    Object::move(offset);
}

void Character::setScale(sf::Vector2f factor)
{
    mSprite.setScale(factor);
    Object::setScale(factor);
}

void Character::setRotation(float angle)
{
    mSprite.setRotation(angle);
    Object::setRotation(angle);
}

void Character::rotate(float angle)
{
    mSprite.rotate(angle);
    Object::rotate(angle);
}

unsigned int Character::getMaxHP() const { return mMaxHP; }

unsigned int Character::getHP() const { return mHP; }

sf::Sprite& Character::getSprite() { return mSprite; }

/*auto Character::getEquipment() const -> const Equipment&
{}*/

/*auto Character::getCurrentWeapon() const -> const Weapon&
{}*/

/*auto Character::getExperienceManager() const -> const ExperienceManager&
{}*/
