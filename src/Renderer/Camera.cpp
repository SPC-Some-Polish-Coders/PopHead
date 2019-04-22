#include "camera.hpp"
#include "Utilities/math.hpp"
#include "Utilities/random.hpp"

PopHead::Renderer::Camera::Camera(sf::Vector2f center, sf::Vector2f size)
	: mView(center, size), mCenterWithoutShake(center)
{
}

void PopHead::Renderer::Camera::setCenter(sf::Vector2f center)
{
	mCenterWithoutShake = center;
	mView.setCenter(center);
}

void PopHead::Renderer::Camera::shake(float shakeStrengthLoss)
{
	if (mShakeStrength > 0.f) {
		const float randomNumber = Utilities::Random::generateNumber(-mShakeStrength, mShakeStrength);
		mView.setCenter(mCenterWithoutShake.x + randomNumber, mCenterWithoutShake.y + randomNumber);
		mShakeStrength -= shakeStrengthLoss;
	}
}

void PopHead::Renderer::Camera::move(sf::Vector2f center, float speed)
{
	mCenterWithoutShake = Utilities::Math::lerp(mCenterWithoutShake, center, speed);
	mView.setCenter(mCenterWithoutShake);
}

void PopHead::Renderer::Camera::applyTo(sf::RenderTarget& renderTarget) const
{
	renderTarget.setView(mView);
}
