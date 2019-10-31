#include "gun.hpp"
#include "Characters/enemy.hpp"
#include "Logs/logs.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Utilities/rect.hpp"
#include <GL/glew.h>
#include <cmath>
#include <array>

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
	auto shotArea = getShotArea();
	std::vector<Character*> charactersInShotArea;
	for(auto& object : mNodeWithAttackableObjects.getChildren()) {
		auto character = dynamic_cast<Character*>(object.get());
		if(character == nullptr)
			continue;
		if(shotArea.doPositiveRectsIntersect(character->getTextureBounds()))
			charactersInShotArea.emplace_back(character);
	}
	return charactersInShotArea;
}

FloatRect Bullet::getShotArea()
{
	sf::Vector2f topLeftCorner = getShotAreaTopLeftCorner();
	sf::Vector2f size = getShotAreaSize();
	return FloatRect(topLeftCorner.x, topLeftCorner.y, size.x, size.y);
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
	return mStartPosition + (mDirection * static_cast<float>(mTraveledDistance));
}

bool Bullet::wasCharacterShot(Character* character, const sf::Vector2f currentBulletPosition)
{
	const FloatRect hitbox = character->getTextureBounds();
	return hitbox.containsIncludingBounds(currentBulletPosition);
}

Gun::Gun(SoundPlayer& soundPlayer, const Texture& texture, const float damage)
	:GameObject("gun")
	,mGunSprite(texture, "gun")
	,mGunShotShader()
	,mCurrentPlayerDirection({1.f, 0.f})
	,mGunShader(ShaderLibrary::getInstance().get("textureSprite"))
	,mSoundPlayer(soundPlayer)
	,mDamage(damage)
	,mShouldDisplayShotGraphics(false)
	,mShouldDisplayGunSprite(false)
	,mHasSetShotGraphicsIndexBuffer(false)
{
	mGunShotShader.loadFromFile("resources/shaders/gunShot.vs.glsl", "resources/shaders/gunShot.fs.glsl");
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

void Gun::initializeShotGraphics(const Bullet& bullet, const sf::Vector2f rightHandPosition)
{
	updateShotGraphicsVertexBuffer(bullet);
	updateGunTextureRect();
	updateGunSpriteFlipping();
	updateGunSpritePosition();

	mShouldDisplayShotGraphics = true;
	mShouldDisplayGunSprite = true;
}

void Gun::updateShotGraphicsVertexBuffer(const Bullet& bullet)
{
	// set index buffer
	if(!mHasSetShotGraphicsIndexBuffer) {
		std::array<unsigned, 2> indices{0, 1};
		IndexBuffer ibo = createIndexBuffer();
		setData(ibo, indices.data(), 2);
		mShotGraphicsVertexArray.setIndexBuffer(ibo);
		mHasSetShotGraphicsIndexBuffer = true;
	}
	// set vertex buffer
	const sf::Vector2f bulletStartPosition = mCurrentPlayerDirection == sf::Vector2f(1, 0) ? bullet.getStartPosition() + sf::Vector2f(7, 0) : bullet.getStartPosition();
	const sf::Vector2f bulletEndPosition = bullet.getCurrentPosition();
	std::array<float, 4> vertices{
		bulletStartPosition.x, bulletStartPosition.y,
		bulletEndPosition.x, bulletEndPosition.y
	};
	VertexBuffer vbo = createVertexBuffer();
	setData(vbo, vertices.data(), vertices.size() * sizeof(float), DataUsage::dynamicDraw);
	mShotGraphicsVertexArray.setVertexBuffer(vbo, VertexBufferLayout::position2);
}

void Gun::updateCurrent(const sf::Time delta)
{
	if(mTimeFromTrigerPull.getElapsedTime().asSeconds() > 0.02f && mTimeFromTrigerPull.getElapsedTime().asSeconds() < 1.f) {
		mShouldDisplayShotGraphics = false;

		updateGunTextureRect(16);
		updateGunSpriteFlipping();
		updateGunSpritePosition();
	}
	if(mTimeFromTrigerPull.getElapsedTime().asSeconds() > 1.f)
		mShouldDisplayGunSprite = false;
}

void Gun::updateGunTextureRect(const int offsetX)
{
	auto& vbo = mGunSprite.mVertexArray.getVertexBuffer();
	if(mCurrentPlayerDirection == sf::Vector2f(1.f, 0.f) || mCurrentPlayerDirection == sf::Vector2f(-1.f, 0.f))
		setTextureRect(vbo, {offsetX, 0, 13, 8}, mGunSprite.mSize);
	else if(mCurrentPlayerDirection == sf::Vector2f(0.f, 1.f) || mCurrentPlayerDirection == sf::Vector2f(0.f, -1.f))
		setTextureRect(vbo, {offsetX, 10, 13, 11}, mGunSprite.mSize);
	else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, -0.7f) || mCurrentPlayerDirection == sf::Vector2f(0.7f, -0.7f))
		setTextureRect(vbo, {offsetX, 21, 13, 11}, mGunSprite.mSize);
	else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, 0.7f) || mCurrentPlayerDirection == sf::Vector2f(0.7f, 0.7f))
		setTextureRect(vbo, {offsetX, 34, 13, 11}, mGunSprite.mSize);
}

void Gun::updateGunSpriteFlipping()
{
	if(mCurrentPlayerDirection.x > 0)
		mGunScale = {1.f, 1.f};
	else if(mCurrentPlayerDirection.x < 0)
		mGunScale = {-1.f, 1.f};
	else if(mCurrentPlayerDirection == sf::Vector2f(0, -1))
		mGunScale = {1.f, 1.f};
	else if(mCurrentPlayerDirection == sf::Vector2f(0, 1))
		mGunScale = {1.f, -1.f};
}

void Gun::updateGunSpritePosition()
{
	const sf::Vector2f rightHandPosition = getRightHandPosition();
	if(mCurrentPlayerDirection == sf::Vector2f(1.f, 0.f))
		mGunPosition = rightHandPosition + sf::Vector2f(0.f, -3.f);
	else if(mCurrentPlayerDirection == sf::Vector2f(-1.f, 0.f))
		mGunPosition = rightHandPosition + sf::Vector2f(3.f, -3.f);
	else if(mCurrentPlayerDirection == sf::Vector2f(0.f, -1.f))
		mGunPosition = rightHandPosition + sf::Vector2f(-3.f, 1.f);
	else if(mCurrentPlayerDirection == sf::Vector2f(0.f, 1.f))
		mGunPosition = rightHandPosition + sf::Vector2f(-3.f, 9.f);
	else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, -0.7f))
		mGunPosition = rightHandPosition + sf::Vector2f(0.f, -8.f);
	else if(mCurrentPlayerDirection == sf::Vector2f(0.7f, -0.7f))
		mGunPosition = rightHandPosition + sf::Vector2f(0.f, -8.f);
	else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, 0.7f))
		mGunPosition = rightHandPosition + sf::Vector2f(3.f, -3.f);
	else if(mCurrentPlayerDirection == sf::Vector2f(0.7f, 0.7f))
		mGunPosition = rightHandPosition + sf::Vector2f(-3.f, -1.f);
	else
		mGunPosition = rightHandPosition;
}

sf::Vector2f Gun::getRightHandPosition()
{
	if(mCurrentPlayerDirection == sf::Vector2f(1, 0))
		return {16, 7};
	else if(mCurrentPlayerDirection == sf::Vector2f(-1, 0))
		return {-2, 7};
	else if(mCurrentPlayerDirection == sf::Vector2f(0, 1))
		return {10, 18};
	else if(mCurrentPlayerDirection == sf::Vector2f(0, -1))
		return {9, -12};
	else if(mCurrentPlayerDirection == sf::Vector2f(0.7f, -0.7f))
		return {15, -5};
	else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, -0.7f))
		return {-1, -1};
	else if(mCurrentPlayerDirection == sf::Vector2f(0.7f, 0.7f))
		return {17, 17};
	else if(mCurrentPlayerDirection == sf::Vector2f(-0.7f, 0.7f))
		return {-3, 17};
	else
		PH_EXCEPTION("Direction vector like this shouldn't exist.");
}

void Gun::drawCurrent(sf::Transform transform)
{
	if(mShouldDisplayShotGraphics)
		Renderer::submit(mShotGraphicsVertexArray, mGunShotShader, sf::Transform::Identity, {1000, 1000}, DrawPrimitive::Lines);

	transform.translate(mGunPosition);
	transform.scale(mGunScale * 10.f);
	if(mShouldDisplayGunSprite)
		Renderer::submit(mGunSprite, *mGunShader, transform);
}

}
