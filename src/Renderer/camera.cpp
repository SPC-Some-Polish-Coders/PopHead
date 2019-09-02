#include "camera.hpp"
#include "Utilities/math.hpp"
#include "Utilities/random.hpp"

#include <iostream>

namespace ph {

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

auto Camera::getBounds() const -> sf::FloatRect
{
	const sf::Vector2f topLeftCornerPosition(mCenterWithoutShake.x - getSize().x / 2, mCenterWithoutShake.y - getSize().y / 2);
	return sf::FloatRect(topLeftCornerPosition.x, topLeftCornerPosition.y, getSize().x, getSize().y);
}

void Camera::shake(float shakeStrengthLoss)
{
	if (mShakeStrength > 0.f) {
		const float randomNumber = Random::generateNumber(-mShakeStrength, mShakeStrength);
		mView.setCenter(mCenterWithoutShake.x + randomNumber, mCenterWithoutShake.y + randomNumber);
		mShakeStrength -= shakeStrengthLoss;
	}
}

void Camera::move(sf::Vector2f center, float speed)
{
	mCenterWithoutShake = Math::lerp(mCenterWithoutShake, center, speed);
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

}
