#include "car.hpp"
#include "Renderer/Shaders/shaderLibary.hpp"

namespace ph {

Car::Car(const float acceleration, const float slowingDown, const sf::Vector2f direction, ph::Texture& texture)
	:GameObject("car")
	,mSize(texture.getSize())
	,mTexture(texture)
	,mDirection(direction)
	,mAcceleration(acceleration)
	,mSlowingDown(slowingDown)
	,mVelocity(0.f)
	,mShouldSpeedUp(false)
	,mShouldSlowDown(false)
{
}

void Car::updateCurrent(const sf::Time delta)
{
	if(mShouldSpeedUp)
		mVelocity += mAcceleration * delta.asSeconds();

	if(mShouldSlowDown)
		mVelocity -= mSlowingDown * delta.asSeconds();

	constexpr float frictionForce = 0.1f;
	mVelocity -= frictionForce * delta.asSeconds();

	if(mVelocity < 1)
		mVelocity = 0;

	move(mVelocity * mDirection * delta.asSeconds());
}

void Car::drawCurrent(sf::Transform transform)
{
	Renderer::submitQuad(mTexture, getPosition(), mSize);
}

}
