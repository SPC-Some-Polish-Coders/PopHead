#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class Car : public GameObject
{
public:
	Car(const float acceleration, const float slowingDown, const sf::Vector2f direction, sf::Texture&);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::RenderTarget&, const sf::RenderStates) const override;

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
