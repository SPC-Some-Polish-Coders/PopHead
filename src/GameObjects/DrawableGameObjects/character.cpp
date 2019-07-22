#include "character.hpp"

#include "Physics/CollisionBody/collisionBody.hpp"
#include "gameData.hpp"

namespace ph {

Character::Character(GameData* gameData, std::string name, Animation animation,
	unsigned int movementSpeed, unsigned int HP, unsigned int maxHP, sf::FloatRect posAndSize, float mass)
	:DrawableGameObject(gameData, name, LayerID::kinematicEntities)
	,mHP(HP)
	,mMaxHP(maxHP)
	,mMovementSpeed(movementSpeed)
	,mAnimation(animation)
	,mCollisionBody(mGameData->getPhysicsEngine().createKinematicBodyAndGetTheReference(posAndSize, mass))
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
	DrawableGameObject::setPosition(position, recursive);
}

void Character::move(sf::Vector2f offset, bool recursive)
{
	mSprite.move(offset);
	mCollisionBody.move(offset);
	DrawableGameObject::move(offset, recursive);
}

void Character::setScale(sf::Vector2f factor, bool recursive)
{
	mSprite.setScale(factor);
	DrawableGameObject::setScale(factor, recursive);
}

void Character::setRotation(float angle, bool recursive)
{
	mSprite.setRotation(angle);
	DrawableGameObject::setRotation(angle, recursive);
}

void Character::rotate(float angle, bool recursive)
{
	mSprite.rotate(angle);
	DrawableGameObject::rotate(angle, recursive);
}

}