#include "character.hpp"

#include "Physics/CollisionBody/collisionBody.hpp"
#include "GameObjects/GameObjectContainers/particlesSystem.hpp"
#include "GameObjects/DrawableGameObjects/particles.hpp"
#include "gameData.hpp"

namespace ph {

Character::Character(GameData* gameData, std::string name, Animation animation,
	unsigned movementSpeed, int Hp, unsigned maxHp, sf::FloatRect posAndSize, float mass)
	:DrawableGameObject(gameData->getRenderer(), name, LayerID::kinematicEntities)
	,mGameData(gameData)
	,mHp(Hp)
	,mMaxHp(maxHp)
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

auto Character::getSpriteCenter() -> sf::Vector2f
{
	sf::IntRect spriteRect = getSprite().getTextureRect();
	return { spriteRect.height / 2.f, spriteRect.width / 2.f };
}

void Character::drawBlood()
{
	auto& root = mGameData->getSceneMachine().getScene().getRoot();
	auto& particlesSystem = dynamic_cast<ParticlesSystem&>(root.getChild("particlesSystem"));
	particlesSystem.addChild(std::make_unique<Particles>(mGameData->getRenderer(), getSpriteCenter() + mPosition));
}

void Character::takeDamage(const unsigned damage)
{ 
	//INFO: Temporary solution so the particles don't bug

	if (mTimeFromLastHit.getElapsedTime().asSeconds() > 0.15f) {
		mHp -= damage;
		drawBlood();
		mTimeFromLastHit.restart();
	}
}

}