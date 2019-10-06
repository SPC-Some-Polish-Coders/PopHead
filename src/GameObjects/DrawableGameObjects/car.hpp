#pragma once

#include "GameObjects/gameObject.hpp"
#include "Renderer/vertexArray.hpp"
#include "Renderer/shader.hpp"
#include "Renderer/texture.hpp"
#include <memory>

namespace ph {

class Car : public GameObject
{
public:
	Car(const float acceleration, const float slowingDown, const sf::Vector2f direction, ph::Texture&);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(const sf::Transform transform) override;

	void setVelocity(float velocity) { mVelocity = velocity; }
	float getVelocity() const { return mVelocity; }

	void speedUp() { mShouldSpeedUp = true; }
	void slowDown() { mShouldSlowDown = true; }

private:
	std::shared_ptr<VertexArray> mVertexArray;
	std::shared_ptr<Shader> mShader;
	sf::Vector2f mDirection;
	ph::Texture& mTexture;
	float mVelocity;
	const float mAcceleration;
	const float mSlowingDown;
	bool mShouldSpeedUp;
	bool mShouldSlowDown;
};

}
