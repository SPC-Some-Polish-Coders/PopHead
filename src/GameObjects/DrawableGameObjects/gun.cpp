#include "gun.hpp"
#include "character.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

Bullet::Bullet(const GameObject& enemiesNode, const sf::Vector2f direction, const sf::Vector2f startPosition,
               const unsigned  damage, const unsigned range)
	:mDirection(direction)
	,mStartPosition(startPosition)
	,mEnemiesNode(enemiesNode)
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
	while(isBulletStillInItsRange()) {
		for(auto& enemy : mEnemiesNode.getChildren()) {
			auto& e = dynamic_cast<Character&>(*enemy);
			if(!e.isDead() && wasEnemyShot(e))
				return &e;
		}
		++mTraveledDistance;
	}
	return nullptr;
}

bool Bullet::isBulletStillInItsRange()
{
	return mTraveledDistance < mRange;
}

bool Bullet::wasEnemyShot(Character& character)
{
	const sf::FloatRect hitbox = character.getGlobalBounds();
	const sf::Vector2f currentPosition = mStartPosition + (mDirection * static_cast<float>(mTraveledDistance));
	return Math::isPointInsideRect(currentPosition, hitbox);
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
	auto& player = getParent();
	auto& root = player.getParent();
	auto& enemies = root.getChild("enemy_container");
	setGunPositionToRightHand(shotDirection);
	const Bullet bullet(enemies, shotDirection, getPosition(), 50, 250);
	initializeShotGraphics(bullet);
	mTimeFromTrigerPull.restart();
}

void Gun::setGunPositionToRightHand(const sf::Vector2f shotDirection)
{
	sf::Vector2f position = getPosition();

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

	setPosition(position);
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