#include "gun.hpp"
#include "gameData.hpp"
#include "Utilities/debug.hpp"

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
			if(wasEnemyShot(e))
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
	const auto& sprite = character.getSprite();
	const sf::FloatRect hitbox = sprite.getGlobalBounds();
	const sf::Vector2f currentPosition = mStartPosition + (mDirection	* static_cast<float>(mTraveledDistance));
	return Math::isPointInsideRect(currentPosition, hitbox);
}

Gun::Gun(GameData* const gameData, const float damage)
	:DrawableGameObject(gameData, "gun", LayerID::kinematicEntities)
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
	const Bullet bullet(enemies, shotDirection, mPosition, 50, 250);
	initializeShotGraphics(bullet);
	mTimeFromTrigerPull.restart();
}

void Gun::setGunPositionToRightHand(const sf::Vector2f shotDirection)
{
	if(shotDirection == sf::Vector2f(1, 0))
		mPosition += {20, 20};
	else if(shotDirection == sf::Vector2f(-1, 0))
		mPosition += {5, 15};
	else if(shotDirection == sf::Vector2f(0, 1))
		mPosition += {3, 20};
	else if(shotDirection == sf::Vector2f(0, -1))
		mPosition += {15, 15};
	else if(shotDirection == sf::Vector2f(0.7f, -0.7f))
		mPosition += {20, 3};
	else if(shotDirection == sf::Vector2f(-0.7f, -0.7f))
		mPosition += {3, 3};
	else if(shotDirection == sf::Vector2f(0.7f, 0.7f))
		mPosition += {10, 20};
	else if(shotDirection == sf::Vector2f(-0.7f, 0.7f))
		mPosition += {0, 10};
	else
		PH_EXCEPTION("Direction vector like this shouldn't exist.");
}

void Gun::initializeShotGraphics(const Bullet& bullet)
{
	mShotGraphics[0].position = bullet.getStartPosition();
	mShotGraphics[1].position = bullet.getCurrentPosition();
}

void Gun::update(const sf::Time delta)
{
	if(mTimeFromTrigerPull.getElapsedTime().asSeconds() > 0.02f)
		resetShotGraphics();
}

void Gun::resetShotGraphics()
{
	mShotGraphics[0].position = {0, 0};
	mShotGraphics[1].position = {0, 0};
}

void Gun::draw(sf::RenderTarget& target, const sf::RenderStates) const
{
	target.draw(mShotGraphics.data(), 2, sf::Lines);
}

}