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
	,mNodeWithAtackableObjects(enemiesNode)
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
	sf::Vector2f topLeftCorner(
		mDirection.x < 0 ? mStartPosition.x - mRange : mStartPosition.x,
		mDirection.y < 0 ? mStartPosition.y - mRange : mStartPosition.y
	);
	sf::Vector2f size(
		std::abs(mDirection.x == 0 ? 1 : mDirection.x * mRange),
		std::abs(mDirection.y == 0 ? 1 : mDirection.y * mRange)
	);
	sf::FloatRect shotArea(topLeftCorner.x, topLeftCorner.y, size.x, size.y);
	
	std::vector<Character*> charactersOnShotLine;
	
	for(auto& object : mNodeWithAtackableObjects.getChildren()) {
		auto character = dynamic_cast<Character*>(object.get());
		if(character == nullptr)
			continue;
		if(Math::areTheyOverlapping(shotArea, character->getGlobalBounds()))
			charactersOnShotLine.emplace_back(character);
	}

	if(charactersOnShotLine.empty())
		return nullptr;

	while(isBulletStillInItsRange()) {
		const sf::Vector2f currentBulletPosition = mStartPosition + (mDirection * static_cast<float>(mTraveledDistance));
		for(auto& c : charactersOnShotLine) {
			if(wasCharacterShot(c, currentBulletPosition) && c->isAtackable() && !c->isDead())
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
	mGameData->getSoundPlayer().playAmbientSound("sounds/barretaShot.wav");
	auto& standingObjects = mRoot->getChild("LAYER_standingObjects");
	const sf::Vector2f rightHandPosition = getRightHandPosition(shotDirection);
	const Bullet bullet(standingObjects, shotDirection, rightHandPosition, 50, 250);
	initializeShotGraphics(bullet);
	mTimeFromTrigerPull.restart();
}

sf::Vector2f  Gun::getRightHandPosition(const sf::Vector2f shotDirection)
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
