#include "gun.hpp"
#include "Characters/enemy.hpp"
#include "Logs/logs.hpp"
#include "Audio/Sound/soundPlayer.hpp"
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
		mDirection.x == 0 ? 1 : static_cast<float>(mRange),
		mDirection.y == 0 ? 1 : static_cast<float>(mRange)
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

Gun::Gun(SoundPlayer& soundPlayer, const sf::Texture& texture, const float damage)
	:GameObject("gun")
	,mGunSprite(texture)
	,mSoundPlayer(soundPlayer)
	,mDamage(damage)
	,mShouldDisplayGunAndShotGraphics(false)
{
}

void Gun::shoot(const sf::Vector2f shotDirection)
{
	mSoundPlayer.playAmbientSound("sounds/pistolShot.ogg");
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");
	const sf::Vector2f rightHandPosition = getRightHandPosition(shotDirection);
	const sf::Vector2f rightHandGlobalPosition(rightHandPosition + getWorldPosition());
	const Bullet bullet(*standingObjects, shotDirection, rightHandGlobalPosition, 50, 250);
	initializeShotGraphics(bullet, rightHandPosition, shotDirection);
	mTimeFromTrigerPull.restart();
}

sf::Vector2f Gun::getRightHandPosition(const sf::Vector2f shotDirection)
{
	if(shotDirection == sf::Vector2f(1, 0))
		return {14, 7};
	else if(shotDirection == sf::Vector2f(-1, 0))
		return {0, 7};
	else if(shotDirection == sf::Vector2f(0, 1))
		return {10, 15};
	else if(shotDirection == sf::Vector2f(0, -1))
		return {10, -10};
	else if(shotDirection == sf::Vector2f(0.7f, -0.7f))
		return {17, -3};
	else if(shotDirection == sf::Vector2f(-0.7f, -0.7f))
		return {-3, -3};
	else if(shotDirection == sf::Vector2f(0.7f, 0.7f))
		return {17, 17};
	else if(shotDirection == sf::Vector2f(-0.7f, 0.7f))
		return {-3, 17};
	else
		PH_EXCEPTION("Direction vector like this shouldn't exist.");
}

void Gun::initializeShotGraphics(const Bullet& bullet, const sf::Vector2f rightHandPostion, const sf::Vector2f shotDirection)
{
	mShotGraphics[0].position = shotDirection == sf::Vector2f(1, 0) ? bullet.getStartPosition() + sf::Vector2f(7, 0) : bullet.getStartPosition();
	mShotGraphics[1].position = bullet.getCurrentPosition();

	if(shotDirection == sf::Vector2f(1.f, 0.f) || shotDirection == sf::Vector2f(-1.f, 0.f))
		mGunSprite.setTextureRect({0, 0, 12, 7});
	else if(shotDirection == sf::Vector2f(0.f, 1.f) || shotDirection == sf::Vector2f(0.f, -1.f))
		mGunSprite.setTextureRect({14, 0, 3, 8});
	else if(shotDirection == sf::Vector2f(-0.7f, - 0.7f) || shotDirection == sf::Vector2f(0.7f, -0.7f))
		mGunSprite.setTextureRect({19, 0, 8, 10});
	else if(shotDirection == sf::Vector2f(-0.7f, 0.7f) || shotDirection == sf::Vector2f(0.7f, 0.7f))
		mGunSprite.setTextureRect({0, 9, 8, 11});

	if(shotDirection.x > 0)
		mGunSprite.setScale({1.f, 1.f});
	else if(shotDirection.x < 0)
		mGunSprite.setScale({-1.f, 1.f});

	if(shotDirection == sf::Vector2f(1.f, 0.f))
		mGunSprite.setPosition(rightHandPostion + sf::Vector2f(0.f, -3.f));
	else if(shotDirection == sf::Vector2f(-1.f, 0.f))
		mGunSprite.setPosition(rightHandPostion + sf::Vector2f(3.f, -3.f));
	else if(shotDirection == sf::Vector2f(-0.7f, -0.7f))
		mGunSprite.setPosition(rightHandPostion + sf::Vector2f(2.f, -6.f));
	else if(shotDirection == sf::Vector2f(0.7f, -0.7f))
		mGunSprite.setPosition(rightHandPostion + sf::Vector2f(-2.f, -6.f));
	else if(shotDirection == sf::Vector2f(-0.7f, 0.7f))
		mGunSprite.setPosition(rightHandPostion + sf::Vector2f(2.f, 0.f));
	else if(shotDirection == sf::Vector2f(0.7f, 0.7f))
		mGunSprite.setPosition(rightHandPostion + sf::Vector2f(-2.f, 0.f));
	else
		mGunSprite.setPosition(rightHandPostion);

	mShouldDisplayGunAndShotGraphics = true;
}

void Gun::updateCurrent(const sf::Time delta)
{
	if(mTimeFromTrigerPull.getElapsedTime().asSeconds() > 0.02f)
		mShouldDisplayGunAndShotGraphics = false;
}

void Gun::drawCurrent(sf::RenderTarget& target, const sf::RenderStates states) const
{
	if(mShouldDisplayGunAndShotGraphics) {
		target.draw(mShotGraphics.data(), 2, sf::Lines);
		target.draw(mGunSprite, states);
	}
}

}
