#include "gun.hpp"
#include "Characters/enemy.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"
#include <cmath>

namespace ph {

Bullet::Bullet(const GameObject& enemiesNode, const sf::Vector2f direction, const sf::Vector2f startPosition,
               const unsigned damage, const unsigned range)
	:mDirection(direction)
	,mStartPosition(startPosition)
	,mNodeWithAttackableObjects(enemiesNode)
	,mTraveledDistance(1)
	,mRange(range)
	,mDamage(damage)
{
	auto* characterWhoWasShot = getCharacterWhoWasShot();
	if(characterWhoWasShot == nullptr)
		return;
	characterWhoWasShot->takeDamage(mDamage);
}

auto Bullet::getCharacterWhoWasShot() -> Character*
{
	auto charactersInShotArea = getCharactersInShotArea();
	if(charactersInShotArea.empty()) {
		mTraveledDistance = mRange;
		return nullptr;
	}
	return getFirstCharacterOnShotLine(charactersInShotArea);
}

auto Bullet::getCharactersInShotArea() -> std::vector<Character*>
{
	sf::FloatRect shotArea = getShotArea();
	std::vector<Character*> charactersInShotArea;
	for(auto& object : mNodeWithAttackableObjects.getChildren()) {
		auto character = dynamic_cast<Character*>(object.get());
		if(character == nullptr)
			continue;
		if(Math::areTheyOverlapping(shotArea, character->getGlobalBounds()))
			charactersInShotArea.emplace_back(character);
	}
	return charactersInShotArea;
}

sf::FloatRect Bullet::getShotArea()
{
	sf::Vector2f topLeftCorner = getShotAreaTopLeftCorner();
	sf::Vector2f size = getShotAreaSize();
	return sf::FloatRect(topLeftCorner.x, topLeftCorner.y, size.x, size.y);
}

sf::Vector2f Bullet::getShotAreaTopLeftCorner() const
{
	return sf::Vector2f(
		mDirection.x < 0 ? mStartPosition.x - mRange : mStartPosition.x,
		mDirection.y < 0 ? mStartPosition.y - mRange : mStartPosition.y
	);
}

sf::Vector2f Bullet::getShotAreaSize() const
{
	return sf::Vector2f(
		mDirection.x == 0 ? 1 : mRange,
		mDirection.y == 0 ? 1 : mRange
	);
}

auto Bullet::getFirstCharacterOnShotLine(std::vector<Character*> charactersInShotArea) -> Character*
{
	while(isBulletStillInItsRange()) {
		const sf::Vector2f currentBulletPosition = getCurrentPosition();
		for(auto& c : charactersInShotArea) {
			if(wasCharacterShot(c, currentBulletPosition) && c->isAttackable())
				return c;
		}
		mTraveledDistance += 5;
	}
	return nullptr;
}

bool Bullet::isBulletStillInItsRange()
{
	return mTraveledDistance < mRange;
}

auto Bullet::getCurrentPosition() const -> const sf::Vector2f
{
	return  mStartPosition + (mDirection * static_cast<float>(mTraveledDistance));
}

bool Bullet::wasCharacterShot(Character* character, const sf::Vector2f currentBulletPosition)
{
	const sf::FloatRect hitbox = character->getGlobalBounds();
	return Math::isPointInsideRect(currentBulletPosition, hitbox);
}

Gun::Gun(GameData* const gameData, const float damage)
	:GameObject("gun")
	,mGameData(gameData)
	,mDamage(damage)
{
}

void Gun::shoot(const sf::Vector2f shotDirection)
{
	mGameData->getSoundPlayer().playAmbientSound("sounds/pistolShot.ogg");
	auto& standingObjects = mRoot->getChild("LAYER_standingObjects");
	const sf::Vector2f rightHandPosition = getRightHandPosition(shotDirection);
	const Bullet bullet(standingObjects, shotDirection, rightHandPosition, 50, 250);
	initializeShotGraphics(bullet);
	mTimeFromTrigerPull.restart();
}

sf::Vector2f Gun::getRightHandPosition(const sf::Vector2f shotDirection)
{
	sf::Vector2f position = getWorldPosition();

	if(shotDirection == sf::Vector2f(1, 0))
		position += {20, 20};
	else if(shotDirection == sf::Vector2f(-1, 0))
		position += {5, 15};
	else if(shotDirection == sf::Vector2f(0, 1))
		position += {3, 20};
	else if(shotDirection == sf::Vector2f(0, -1))
		position += {15, 15};
	else if(shotDirection == sf::Vector2f(0.7f, -0.7f))
		position += {20, 3};
	else if(shotDirection == sf::Vector2f(-0.7f, -0.7f))
		position += {3, 3};
	else if(shotDirection == sf::Vector2f(0.7f, 0.7f))
		position += {10, 20};
	else if(shotDirection == sf::Vector2f(-0.7f, 0.7f))
		position += {0, 10};
	else
		PH_EXCEPTION("Direction vector like this shouldn't exist.");

	return position;
}

void Gun::initializeShotGraphics(const Bullet& bullet)
{
	mShotGraphics[0].position = bullet.getStartPosition();
	mShotGraphics[1].position = bullet.getCurrentPosition();
}

void Gun::updateCurrent(const sf::Time delta)
{
	if(mTimeFromTrigerPull.getElapsedTime().asSeconds() > 0.02f)
		resetShotGraphics();
}

void Gun::resetShotGraphics()
{
	mShotGraphics[0].position = {0, 0};
	mShotGraphics[1].position = {0, 0};
}

void Gun::drawCurrent(sf::RenderTarget& target, const sf::RenderStates) const
{
	target.draw(mShotGraphics.data(), 2, sf::Lines);
}

}
