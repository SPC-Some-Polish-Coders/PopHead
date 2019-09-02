#include "GameObjects/DrawableGameObjects/melee.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "gameData.hpp"

namespace ph {

Swing::Swing(const GameObject& nodeWithAttackableObjects, const sf::Vector2f direction, const sf::Vector2f position,
	const float damage, const float range, const float rotationRange)
	:mNodeWithAttackableObjects(nodeWithAttackableObjects)
	,mDirection(direction)
	,mStartPosition(position)
	,mDamage(damage)
	,mRange(range)
	,mRotationRange(rotationRange)
	,mRotation(0.f)
{
	setMeeleWeaponStartingPosition(direction);
	handleHitCharacters();
}

void Swing::setMeeleWeaponStartingPosition(const sf::Vector2f attackDirection)
{
	sf::Transform rotation;
	rotation.rotate(-mRotationRange / 2, mStartPosition);
	mHitArea[0].position = mStartPosition;
	mHitArea[1].position = mStartPosition + (attackDirection * mRange);
	mHitArea[1] = rotation.transformPoint(mHitArea[1].position);
}

void Swing::handleHitCharacters()
{
	auto attackableCharactersInHitArea = getAttackableCharactersInHitArea();
	if(attackableCharactersInHitArea.empty())
		return;
	auto firstChracterWhoWouldBeHit = getFirstCharacterWhoWouldBeHit(attackableCharactersInHitArea);
	if(firstChracterWhoWouldBeHit != nullptr)
		firstChracterWhoWouldBeHit->takeDamage(static_cast<unsigned>(mDamage));
}

auto Swing::getAttackableCharactersInHitArea() const -> std::vector<Character*>
{
	const sf::FloatRect hitArea(mStartPosition.x - 20, mStartPosition.y - 20, 40, 40);
	std::vector<Character*> attackableCharactersInHitArea;

	for(auto& attackableObject : mNodeWithAttackableObjects.getChildren()) {
		auto* c = dynamic_cast<Character*>(attackableObject.get());
		if(c == nullptr)
			continue;
		if(c->isAttackable() && Math::areTheyOverlapping(hitArea, c->getGlobalBounds()))
			attackableCharactersInHitArea.emplace_back(c);
	}
	return attackableCharactersInHitArea;
}

auto Swing::getFirstCharacterWhoWouldBeHit(const std::vector<Character*>& attackableCharactersInHitArea) -> Character*
{
	while(mRotation < 100) {
		incrementRotation();
		for(auto* attackableCharacter : attackableCharactersInHitArea) {
			if(wasCharacterHit(attackableCharacter))
				return attackableCharacter;
		}
	}
	return nullptr;
}

void Swing::incrementRotation()
{
	mRotation += 5.f;
	sf::Transform rotation;
	rotation.rotate(5.f, mStartPosition);
	mHitArea[1] = rotation.transformPoint(mHitArea[1].position);
}

bool Swing::wasCharacterHit(Character* character)
{
	const sf::FloatRect hitbox = character->getGlobalBounds();
	return Math::isPointInsideRect(mHitArea[1].position, hitbox);
}

MeleeWeapon::MeleeWeapon(GameData* const gameData, const float damage, const float range, const float rotatationRange)
	:GameObject("sword")
	,mSprite(gameData->getTextures().get("textures/others/baseballStick.png"))
	,mGameData(gameData)
	,mDamage(damage)
	,mRange(range)
	,mRotationRange(rotatationRange)
	,mRotationFromStart(0.f)
	,mShouldBeDrawn(false)
{
}

void MeleeWeapon::attack(const sf::Vector2f attackDirection)
{
	mGameData->getSoundPlayer().playAmbientSound("sounds/swordAttack.wav");
	setOrigin(0.f, 12.f);
	setRotation(getStartAttackRotation(attackDirection));
	sf::Vector2f rightHandLocalPosition = getRightHandLocalPosition(attackDirection);
	setPosition(rightHandLocalPosition);
	auto& standingObjects = mRoot->getChild("LAYER_standingObjects");
	Swing swing(standingObjects, attackDirection, rightHandLocalPosition + getWorldPosition(), mDamage, mRange, mRotationRange);
	mShouldBeDrawn = true;

	if (mEndOfMelle != nullptr)
	{
		mGameData->getPhysicsEngine().removeKinematicBody(*mEndOfMelle);
		mEndOfMelle = nullptr;
	}
	sf::Transform local;
	local.rotate(getRotation());
	sf::Vector2f endOfMeleePosition = getWorldPosition() + local.transformPoint(sf::Vector2f(12.f, -12.f));
	sf::FloatRect endOfMeleeRect({ endOfMeleePosition - sf::Vector2f(1.f, 8.f) }, { 2.f, 2.f });
	const float massOfKinematicObject = 50;
	mEndOfMelle = &(mGameData->getPhysicsEngine().createKinematicBodyAndGetTheReference(endOfMeleeRect, massOfKinematicObject));
}

float MeleeWeapon::getStartAttackRotation(const sf::Vector2f attackDirection) const
{
	if(attackDirection == sf::Vector2f(1, 0))
		return 0.f;
	else if(attackDirection == sf::Vector2f(-1, 0))
		return 180.f;
	else if(attackDirection == sf::Vector2f(0, 1))
		return 90.f;
	else if(attackDirection == sf::Vector2f(0, -1))
		return -90.f;
	else if(attackDirection == sf::Vector2f(0.7f, -0.7f))
		return -45.f;
	else if(attackDirection == sf::Vector2f(-0.7f, -0.7f))
		return -135.f;
	else if(attackDirection == sf::Vector2f(0.7f, 0.7f))
		return 45.f;
	else if(attackDirection == sf::Vector2f(-0.7f, 0.7f))
		return 135.f;
	else
		PH_UNEXPECTED_SITUATION("Direction vector like this shouldn't exist.");
}

sf::Vector2f MeleeWeapon::getRightHandLocalPosition(const sf::Vector2f attackDirection)
{
	if (attackDirection == sf::Vector2f(1, 0))
		return {14, 17};
	else if(attackDirection == sf::Vector2f(-1, 0))
		return {5, 15};
	else if(attackDirection == sf::Vector2f(0, 1))
		return {3, 20};
	else if(attackDirection == sf::Vector2f(0, -1))
		return {15, 15};
	else if(attackDirection == sf::Vector2f(0.7f, -0.7f))
		return {15, 7};
	else if(attackDirection == sf::Vector2f(-0.7f, -0.7f))
		return {3, 3};
	else if(attackDirection == sf::Vector2f(0.7f, 0.7f))
		return {13, 17};
	else if(attackDirection == sf::Vector2f(-0.7f, 0.7f))
		return {0, 10};
	else
		PH_UNEXPECTED_SITUATION("Direction vector like this shouldn't exist.");
}

void MeleeWeapon::updateCurrent(const sf::Time delta)
{
	if(mShouldBeDrawn) {
		if(mRotationFromStart < mRotationRange) {
			rotate(3.f);
			mRotationFromStart += 3.f;
		}
		else {
			mShouldBeDrawn = false;
			setRotation(0.f);
			mRotationFromStart = 0.f;
			mGameData->getPhysicsEngine().removeKinematicBody(*mEndOfMelle);
			mEndOfMelle = nullptr;
		}
	}
}

void MeleeWeapon::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mShouldBeDrawn)
		target.draw(mSprite, states);
}

}
