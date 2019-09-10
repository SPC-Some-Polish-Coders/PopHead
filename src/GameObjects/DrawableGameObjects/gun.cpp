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
	,mShouldDisplayShotGraphics(false)
	,mShouldDisplayGunSprite(false)
{
}

void Gun::shoot()
{
	mSoundPlayer.playAmbientSound("sounds/pistolShot.ogg");
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");
	const sf::Vector2f rightHandPosition = getRightHandPosition();
	const sf::Vector2f rightHandGlobalPosition(rightHandPosition + getWorldPosition());
	const Bullet bullet(*standingObjects, mCurrentPlayerDirection, rightHandGlobalPosition, 50, 250);
	initializeShotGraphics(bullet, rightHandPosition);
	mTimeFromTrigerPull.restart();
}

sf::Vector2f Gun::getRightHandPosition()
{
	if(mCurrentPlayerDirection == sf::Vector2f(1, 0))
		return {14, 7};
	else if(mCurrentPlayerDirection == sf::Vector2f(-1, 0))
		return {0, 7};
	else if(mCurrentPlayerDirection == sf::Vector2f(0, 1))
		return {10, 15};
	else if(mCurrentPlayerDirection == sf::Vector2f(0, -1))
		return {10, -10};
	else if(mCurrentPlayerDirection == sf::Vector2f(0.7f, -0.7f))
		return {17, -3};
	else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, -0.7f))
		return {-3, -3};
	else if(mCurrentPlayerDirection == sf::Vector2f(0.7f, 0.7f))
		return {17, 17};
	else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, 0.7f))
		return {-3, 17};
	else
		PH_EXCEPTION("Direction vector like this shouldn't exist.");
}

void Gun::initializeShotGraphics(const Bullet& bullet, const sf::Vector2f rightHandPosition)
{
	mShotGraphics[0].position = mCurrentPlayerDirection == sf::Vector2f(1, 0) ? bullet.getStartPosition() + sf::Vector2f(7, 0) : bullet.getStartPosition();
	mShotGraphics[1].position = bullet.getCurrentPosition();

	if(mCurrentPlayerDirection == sf::Vector2f(1.f, 0.f) || mCurrentPlayerDirection == sf::Vector2f(-1.f, 0.f))
		mGunSprite.setTextureRect({0, 0, 13, 10});
	else if(mCurrentPlayerDirection == sf::Vector2f(0.f, 1.f) || mCurrentPlayerDirection == sf::Vector2f(0.f, -1.f))
		mGunSprite.setTextureRect({0, 10, 13, 10});
	else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, - 0.7f) || mCurrentPlayerDirection == sf::Vector2f(0.7f, -0.7f))
		mGunSprite.setTextureRect({0, 21, 13, 10});
	else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, 0.7f) || mCurrentPlayerDirection == sf::Vector2f(0.7f, 0.7f))
		mGunSprite.setTextureRect({0, 34, 13, 10});

	if(mCurrentPlayerDirection.x > 0)
		mGunSprite.setScale({1.f, 1.f});
	else if(mCurrentPlayerDirection.x < 0)
		mGunSprite.setScale({-1.f, 1.f});

	if(mCurrentPlayerDirection == sf::Vector2f(1.f, 0.f))
		mGunSprite.setPosition(rightHandPosition + sf::Vector2f(0.f, -3.f));
	else if(mCurrentPlayerDirection == sf::Vector2f(-1.f, 0.f))
		mGunSprite.setPosition(rightHandPosition + sf::Vector2f(3.f, -3.f));
	else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, -0.7f))
		mGunSprite.setPosition(rightHandPosition + sf::Vector2f(2.f, -6.f));
	else if(mCurrentPlayerDirection == sf::Vector2f(0.7f, -0.7f))
		mGunSprite.setPosition(rightHandPosition + sf::Vector2f(-2.f, -6.f));
	else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, 0.7f))
		mGunSprite.setPosition(rightHandPosition + sf::Vector2f(2.f, 0.f));
	else if(mCurrentPlayerDirection == sf::Vector2f(0.7f, 0.7f))
		mGunSprite.setPosition(rightHandPosition + sf::Vector2f(-2.f, 0.f));
	else
		mGunSprite.setPosition(rightHandPosition);

	mShouldDisplayShotGraphics = true;
	mShouldDisplayGunSprite = true;
}

void Gun::updateCurrent(const sf::Time delta)
{
	if(mTimeFromTrigerPull.getElapsedTime().asSeconds() > 0.02f && mTimeFromTrigerPull.getElapsedTime().asSeconds() < 1.f) {
		mShouldDisplayShotGraphics = false;

		if(mCurrentPlayerDirection == sf::Vector2f(1.f, 0.f) || mCurrentPlayerDirection == sf::Vector2f(-1.f, 0.f))
			mGunSprite.setTextureRect({16, 0, 10, 11});
		else if(mCurrentPlayerDirection == sf::Vector2f(0.f, 1.f) || mCurrentPlayerDirection == sf::Vector2f(0.f, -1.f))
			mGunSprite.setTextureRect({16, 10, 10, 11});
		else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, -0.7f) || mCurrentPlayerDirection == sf::Vector2f(0.7f, -0.7f))
			mGunSprite.setTextureRect({16, 21, 10, 11});
		else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, 0.7f) || mCurrentPlayerDirection == sf::Vector2f(0.7f, 0.7f))
			mGunSprite.setTextureRect({16, 34, 10, 11});

		if(mCurrentPlayerDirection.x > 0)
			mGunSprite.setScale({1.f, 1.f});
		else if(mCurrentPlayerDirection.x < 0)
			mGunSprite.setScale({-1.f, 1.f});

		const sf::Vector2f rightHandPosition = getRightHandPosition();
		if(mCurrentPlayerDirection == sf::Vector2f(1.f, 0.f))
			mGunSprite.setPosition(rightHandPosition + sf::Vector2f(0.f, -3.f));
		else if(mCurrentPlayerDirection == sf::Vector2f(-1.f, 0.f))
			mGunSprite.setPosition(rightHandPosition + sf::Vector2f(3.f, -3.f));
		else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, -0.7f))
			mGunSprite.setPosition(rightHandPosition + sf::Vector2f(2.f, -6.f));
		else if(mCurrentPlayerDirection == sf::Vector2f(0.7f, -0.7f))
			mGunSprite.setPosition(rightHandPosition + sf::Vector2f(-2.f, -6.f));
		else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, 0.7f))
			mGunSprite.setPosition(rightHandPosition + sf::Vector2f(2.f, 0.f));
		else if(mCurrentPlayerDirection == sf::Vector2f(0.7f, 0.7f))
			mGunSprite.setPosition(rightHandPosition + sf::Vector2f(-2.f, 0.f));
		else
			mGunSprite.setPosition(rightHandPosition);
	}
	if(mTimeFromTrigerPull.getElapsedTime().asSeconds() > 1.f)
		mShouldDisplayGunSprite = false;
}

void Gun::drawCurrent(sf::RenderTarget& target, const sf::RenderStates states) const
{
	if(mShouldDisplayShotGraphics)
		target.draw(mShotGraphics.data(), 2, sf::Lines);
	if(mShouldDisplayGunSprite)
		target.draw(mGunSprite, states);
}

}
