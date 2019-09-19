#include "character.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "GameObjects/GameObjectContainers/particlesSystem.hpp"
#include "GameObjects/DrawableGameObjects/particles.hpp"
#include "GameObjects/GameObjectContainers/itemsContainer.hpp"
#include "GameObjects/NonDrawableGameObjects/equipment.hpp"
#include "Utilities/random.hpp"
#include "gameData.hpp"

namespace ph {

Character::Character(GameData* gameData, std::string name, Animation animation,
	unsigned movementSpeed, int Hp, unsigned maxHp, sf::FloatRect posAndSize, float mass, bool isAttackable)
	:GameObject(name)
	,mGameData(gameData)
	,mHp(Hp)
	,mMaxHp(maxHp)
	,mMovementSpeed(movementSpeed)
	,mAnimation(animation)
	,mCollisionBody(mGameData->getPhysicsEngine().createKinematicBodyAndGetTheReference(posAndSize, mass))
	,mIsAttackable(isAttackable)
{
	addChild(std::make_unique<Equipment>());
	dynamic_cast<Equipment*>(getChild("Equipment"))->init();
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Character::dropItems()
{
	dynamic_cast<Equipment*>(getChild("Equipment"))->dropAllItems();
}

void Character::setPosition(sf::Vector2f position)
{
	auto collisionBodyOffset = mCollisionBody.getPosition() - mCollisionBody.getFixedPosition();
	Transformable::setPosition(position);
	mCollisionBody.setPosition(position + collisionBodyOffset);
}

void Character::setHp(unsigned hp)
{
	mHp = hp;
	fixHp();
}

bool Character::isDead() const
{
	return mHp == 0;
}

void Character::move(sf::Vector2f offset)
{
	mCollisionBody.move(offset);
	Transformable::move(offset);
}

auto Character::getSpriteCenter() -> sf::Vector2f
{
	sf::IntRect spriteRect = getSprite().getTextureRect();
	return { spriteRect.height / 2.f, spriteRect.width / 2.f };
}

sf::FloatRect Character::getGlobalBounds() const
{
	return mCollisionBody.getRect();
}

sf::FloatRect Character::getTextureBounds() const
{
	auto transform = getTransform();
	return transform.transformRect(mSprite.getLocalBounds());
}

void Character::pushCharacter(const sf::Vector2f& pushVector)
{
	mCollisionBody.setForceVector(pushVector);
}

void Character::fixHp()
{
	if (mHp > static_cast<int>(mMaxHp))
		mHp = mMaxHp;
}

void Character::addHp(int hp)
{
	if (mHp + hp > static_cast<int>(mMaxHp))
		mHp = mMaxHp;
	else
		mHp += hp;
}

void Character::setMaxHp(unsigned int maxHp)
{
	mMaxHp = maxHp;
	fixHp();
}

int Character::getHp() const
{
	return mHp;
}

void Character::takeDamage(const unsigned damage)
{ 
	if (mTimeSinceLastTakenDamage.getElapsedTime().asSeconds() > 0.15f) {
		if (static_cast<int>(damage) > mHp)
			mHp = 0;
		else
			mHp -= damage;
		drawBlood();
		mTimeSinceLastTakenDamage.restart();
	}
}

void Character::drawBlood()
{
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");
	auto* particlesSystem = dynamic_cast<ParticlesSystem*>(standingObjects->getChild("particlesSystem"));
	particlesSystem->addChild(std::make_unique<Particles>(mGameData->getRenderer(), getSpriteCenter() + getPosition()));
}

void Character::setAnimationState(const std::string& stateName)
{
	if(mAnimation.getCurrentStateName() != stateName) {
		mAnimation.changeState(stateName);
		mAnimation.animate(mSprite);
	}
}

}
