#include "GameObjects/DrawableGameObjects/melee.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "gameData.hpp"

namespace ph {

Swing::Swing(const GameObject& nodeWithAttackableObjects, const sf::Vector2f direction, const sf::Vector2f position,
	const float damage, const float range, const float rotationRange, float attackAngle)
	:mNodeWithAttackableObjects(nodeWithAttackableObjects)
	,mDirection(direction)
	,mStartPosition(position)
	,mDamage(damage)
	,mRange(range)
	,mRotationRange(rotationRange)
	,mRotation(0.f)
	,mAttackAngle(attackAngle)
{
	setMeeleWeaponStartingPosition(direction);
	handleHitCharacters(direction);
}

void Swing::setMeeleWeaponStartingPosition(const sf::Vector2f& attackDirection)
{
	sf::Transform rotation;
	rotation.rotate(-mRotationRange / 2, mStartPosition);
	mHitArea[0].position = mStartPosition;
	mHitArea[1].position = mStartPosition + (attackDirection * mRange);
	mHitArea[1] = rotation.transformPoint(mHitArea[1].position);
}

void Swing::handleHitCharacters(const sf::Vector2f& attackDirection)
{
	auto attackableCharactersInHitArea = getAttackableCharactersInHitArea();
	if(attackableCharactersInHitArea.empty())
		return;

	for (const auto& character : attackableCharactersInHitArea)
	{
		auto nearestPoint = nearestPointOfCharacter(*character);
		auto distance = Math::getDistanceBetweenPoints(mStartPosition, nearestPoint);

		if (distance > mRange)
			continue;

		auto characterAngle = angleOfPointToStart(nearestPoint);
		if (isAngleInAttackRange(characterAngle))
		{
			character->takeDamage(static_cast<unsigned>(mDamage));
			character->pushCharacter(attackDirection * 100.f);
		}
	}
}

sf::Vector2f Swing::nearestPointOfCharacter(const Character& character) const
{
	auto rect = character.getGlobalBounds();
	auto right = rect.left + rect.width;
	auto bottom = rect.top + rect.height;

	bool onLeft = right < mStartPosition.x;
	bool onRight = rect.left > mStartPosition.x;
	bool above = bottom < mStartPosition.y;
	bool under = rect.top > mStartPosition.y;

	bool sameXAxis = !onLeft && !onRight;
	bool sameYAxis = !above && !under;

	if (sameXAxis && !sameYAxis)
	{
		if (under)
			return sf::Vector2f(mStartPosition.x, rect.top);
		else
			return sf::Vector2f(mStartPosition.x, bottom);
	}
	if (!sameXAxis && sameYAxis)
	{
		if (onRight)
			return sf::Vector2f(rect.left, mStartPosition.y);
		else
			return sf::Vector2f(right, mStartPosition.y);
	}
	if (sameXAxis && sameYAxis)
	{
		return mStartPosition;
	}

	if (onLeft && above)
		return sf::Vector2f(right, bottom);
	if (onLeft && under)
		return sf::Vector2f(right, rect.top);
	if (onRight && above)
		return sf::Vector2f(rect.left, bottom);
	if (onRight && under)
		return sf::Vector2f(rect.left, rect.top);

	PH_UNEXPECTED_SITUATION("Unhandled position of a character");
}

float Swing::angleOfPointToStart(sf::Vector2f point) const
{
	point -= mStartPosition;

	float angle = std::atan2f(point.y, point.x);
	angle *= 180.f;
	angle /= 3.14159f;
	if (angle < 0.f)
		angle += 360.f;
	return angle;
}

auto Swing::getAttackableCharactersInHitArea() const -> std::vector<Character*>
{
	const sf::FloatRect hitArea(mStartPosition.x - 50, mStartPosition.y - 50, 100, 100);
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

bool Swing::isAngleInAttackRange(float angle) const
{
	float halfOfRotationRange = mRotationRange / 2.f;
	auto attackRange = std::make_pair(getFixedAngle(mAttackAngle - halfOfRotationRange), getFixedAngle(mAttackAngle + halfOfRotationRange));

	if (attackRange.first < attackRange.second)
		return angle >= attackRange.first && angle <= attackRange.second;
	return angle >= attackRange.second || angle <= attackRange.first;
}

float Swing::getFixedAngle(float angle)
{
	angle -= (static_cast<unsigned>(angle) / 360) * 360.f;
	if (angle < 0.f)
		angle += 360.f;
	return angle;
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

void MeleeWeapon::attack(const sf::Vector2f attackDirection, float attackRotation)
{
	mGameData->getSoundPlayer().playAmbientSound("sounds/swordAttack.wav");
	setOrigin(0.f, 12.f);
	setRotation(getStartAttackRotation(attackDirection));
	sf::Vector2f rightHandLocalPosition = getRightHandLocalPosition(attackDirection);
	setPosition(rightHandLocalPosition);
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");

	auto playerRect = mParent->getGlobalBounds();
	sf::Vector2f centerOfPlayer(playerRect.left + playerRect.width / 2.f, playerRect.top + playerRect.height / 2.f);

	Swing swing(*standingObjects, attackDirection, centerOfPlayer, mDamage, mRange, mRotationRange, attackRotation);
	mShouldBeDrawn = true;
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
		}
	}
}

void MeleeWeapon::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mShouldBeDrawn)
		target.draw(mSprite, states);
}

}
