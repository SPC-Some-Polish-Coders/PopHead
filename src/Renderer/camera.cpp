#include "camera.hpp"
#include "Utilities/math.hpp"
#include "Utilities/random.hpp"

#include <iostream>

using PopHead::Renderer::Camera;

Camera::Camera(sf::Vector2f center, sf::Vector2f size)
	: mView(center, size), mCenterWithoutShake(center)
{
}

void Camera::update(sf::Time delta)
{
	shake(mShakeStrengthLoss * delta.asSeconds());
}

void Camera::setCenter(sf::Vector2f center)
{
	mCenterWithoutShake = center;
	mView.setCenter(center);
}

void Camera::shake(float shakeStrengthLoss)
{
	if (mShakeStrength > 0.f) {
		const float randomNumber = Utilities::Random::generateNumber(-mShakeStrength, mShakeStrength);
		mView.setCenter(mCenterWithoutShake.x + randomNumber, mCenterWithoutShake.y + randomNumber);
		mShakeStrength -= shakeStrengthLoss;
	}
}

void Camera::move(sf::Vector2f center, float speed)
{
	mCenterWithoutShake = Utilities::Math::lerp(mCenterWithoutShake, center, speed);
	mView.setCenter(mCenterWithoutShake);
	updateCameraMoveFromLastFrame();
	updateLastCameraPosition();
}

void Camera::applyTo(sf::RenderTarget& renderTarget) const
{
	renderTarget.setView(mView);
}

void Camera::updateCameraMoveFromLastFrame()
{
	mCameraMoveFromLastFrame = mCenterWithoutShake - mLastFrameCameraCenterPosition;
}

void Camera::updateLastCameraPosition()
{
	mLastFrameCameraCenterPosition = mCenterWithoutShake;
}
