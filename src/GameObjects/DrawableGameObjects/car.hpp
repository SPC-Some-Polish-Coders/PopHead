#pragma once

#include "GameObjects/drawableGameObject.hpp"

namespace ph {

class Car : public DrawableGameObject
{
public:
	Car(Renderer&, const float acceleration, const float slowingDown, const sf::Vector2f direction, sf::Texture&);

	void updateCurrent(const sf::Time delta) override;
	void draw(sf::RenderTarget&, const sf::RenderStates) const override;

	void move(sf::Vector2f offset, bool recursive = true) override;
	void setPosition(sf::Vector2f position, bool recursive = true) override;

	void setVelocity(float velocity) { mVelocity = velocity; }
	float getVelocity() const { return mVelocity; }

	void speedUp() { mShouldSpeedUp = true; }
	void slowDown() { mShouldSlowDown = true; }

private:
	sf::Sprite mSprite;
	sf::Vector2f mDirection;
	float mVelocity;
	const float mAcceleration;
	const float mSlowingDown;
	bool mShouldSpeedUp;
	bool mShouldSlowDown;
};

}
