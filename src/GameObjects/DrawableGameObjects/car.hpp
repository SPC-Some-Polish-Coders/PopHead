#pragma once

#include "GameObjects/gameObject.hpp"
#include "Renderer/renderer.hpp"

namespace ph {

class Car : public GameObject
{
public:
	Car(const float acceleration, const float slowingDown, const sf::Vector2f direction, ph::Texture&);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::Transform transform) override;

	void setVelocity(float velocity) { mVelocity = velocity; }
	float getVelocity() const { return mVelocity; }

	void speedUp() { mShouldSpeedUp = true; }
	void slowDown() { mShouldSlowDown = true; }

private:
	VertexArray mVertexArray;
	Shader* mShader;
	const sf::Vector2i mSize;
	sf::Vector2f mDirection;
	ph::Texture& mTexture;
	float mVelocity;
	const float mAcceleration;
	const float mSlowingDown;
	bool mShouldSpeedUp;
	bool mShouldSlowDown;
};

}
