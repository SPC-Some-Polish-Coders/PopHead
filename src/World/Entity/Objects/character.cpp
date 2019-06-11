#include "character.hpp"

#include "Physics/CollisionBody/bodyType.hpp"

using ph::World::Entity::Character;

Character::Character(ph::Base::GameData* gameData, std::string name, ph::World::Animation animation,
	unsigned int movementSpeed, unsigned int HP, unsigned int maxHP, sf::FloatRect posAndSize, float mass)
	:Object(gameData, name, Renderer::LayerID::kinematicEntities)
	, mHP(HP)
	, mMaxHP(maxHP)
	, mMovementSpeed(movementSpeed)
	, mMotion()
	, mAnimation(animation)
	, mCollisionBody(posAndSize, mass, Physics::BodyType::kinematicBody, this, gameData)
{
}

void Character::atack()
{

}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Character::setPosition(sf::Vector2f position, bool recursive)
{
	mSprite.setPosition(position);
	mCollisionBody.setPosition(position);
	Object::setPosition(position, recursive);
}

void Character::move(sf::Vector2f offset, bool recursive)
{
	mSprite.move(offset);
	mCollisionBody.move(offset);
	Object::move(offset, recursive);
}

void Character::setScale(sf::Vector2f factor, bool recursive)
{
	mSprite.setScale(factor);
	Object::setScale(factor, recursive);
}

void Character::setRotation(float angle, bool recursive)
{
	mSprite.setRotation(angle);
	Object::setRotation(angle, recursive);
}

void Character::rotate(float angle, bool recursive)
{
	mSprite.rotate(angle);
	Object::rotate(angle, recursive);
}
