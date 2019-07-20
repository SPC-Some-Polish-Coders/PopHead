#include "gun.hpp"
#include "gameData.hpp"

namespace ph {

Bullet::Bullet(const Entity& enemiesNode, const sf::Vector2f direction, const sf::Vector2f startPosition,
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
	:Object(gameData, "gun", LayerID::kinematicEntities)
	,mDamage(damage)
{
}

void Gun::shoot(const ShotDirection shotDirection)
{
	mGameData->getSoundPlayer().playAmbientSound("sounds/barretaShot.wav");
	auto& player = getParent();
	auto& root = player.getParent();
	auto& enemies = root.getChild("enemy_container");
	const sf::Vector2f shotDirectionVector = getShotDirectionVector(shotDirection);
	setGunPositionToRightHand(shotDirection);
	const Bullet bullet(enemies, shotDirectionVector, mPosition, 50, 250);
	initializeShotGraphics(bullet);
	mTimeFromTrigerPull.restart();
}

auto Gun::getShotDirectionVector(const ShotDirection shotDirection) const -> const sf::Vector2f
{
	switch(shotDirection)
	{
		case ShotDirection::east:
			return sf::Vector2f(1, 0);
		case ShotDirection::west:
			return sf::Vector2f(-1, 0);
		case ShotDirection::south:
			return sf::Vector2f(0, 1);
		case ShotDirection::north:
			return sf::Vector2f(0, -1);
		case ShotDirection::northEast:
			return sf::Vector2f(1, -1);
		case ShotDirection::northWest:
			return sf::Vector2f(-1, -1);
		case ShotDirection::southEast:
			return sf::Vector2f(1, 1);
		case ShotDirection::southWest:
			return sf::Vector2f(-1, 1);
	}
}

void Gun::setGunPositionToRightHand(const ShotDirection shotDirection)
{
	switch(shotDirection)
	{
	case ShotDirection::east:
		mPosition += {20, 20};
		break;
	case ShotDirection::west:
		mPosition += {5, 15};
		break;
	case ShotDirection::south:
		mPosition += {3, 20};
		break;
	case ShotDirection::north:
		mPosition += {15, 15};
		break;
	case ShotDirection::northEast:
		mPosition += {20, 3};
		break;
	case ShotDirection::northWest:
		mPosition += {3, 3};
		break;
	case ShotDirection::southEast:
		mPosition += {10, 20};
		break;
	case ShotDirection::southWest:
		mPosition += {0, 10};
		break;
	}
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