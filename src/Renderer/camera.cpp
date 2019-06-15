#include "camera.hpp"
#include "Utilities/math.hpp"
#include "Utilities/random.hpp"

#include <iostream>

ph::Camera::Camera(sf::Vector2f center, sf::Vector2f size)
	: mView(center, size), mCenterWithoutShake(center)
{
}

void ph::Camera::update(sf::Time delta)
{
	shake(mShakeStrengthLoss * delta.asSeconds());
}

void ph::Camera::setCenter(sf::Vector2f center)
{
	mCenterWithoutShake = center;
	mView.setCenter(center);
}

void ph::Camera::shake(float shakeStrengthLoss)
{
	if (mShakeStrength > 0.f) {
		const float randomNumber = Random::generateNumber(-mShakeStrength, mShakeStrength);
		mView.setCenter(mCenterWithoutShake.x + randomNumber, mCenterWithoutShake.y + randomNumber);
		mShakeStrength -= shakeStrengthLoss;
	}
}

void ph::Camera::move(sf::Vector2f center, float speed)
{
	mCenterWithoutShake = Math::lerp(mCenterWithoutShake, center, speed);
	mView.setCenter(mCenterWithoutShake);
	updateCameraMoveFromLastFrame();
	updateLastCameraPosition();
}

void ph::Camera::applyTo(sf::RenderTarget& renderTarget) const
{
	renderTarget.setView(mView);
}

void ph::Camera::updateCameraMoveFromLastFrame()
{
	mCameraMoveFromLastFrame = mCenterWithoutShake - mLastFrameCameraCenterPosition;
}

void ph::Camera::updateLastCameraPosition()
{
	mLastFrameCameraCenterPosition = mCenterWithoutShake;
}
