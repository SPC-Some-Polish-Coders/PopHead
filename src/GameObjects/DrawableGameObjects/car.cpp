#include "car.hpp"
#include <array>
#include "Renderer/buffers.hpp"
#include "Renderer/renderer.hpp"

namespace ph {

Car::Car(const float acceleration, const float slowingDown, const sf::Vector2f direction, ph::Texture& texture)
	:GameObject("car")
	,mVertexArray(std::make_shared<VertexArray>())
	,mShader(std::make_shared<Shader>())
	,mTexture(texture)
	,mDirection(direction)
	,mAcceleration(acceleration)
	,mSlowingDown(slowingDown)
	,mVelocity(0.f)
	,mShouldSpeedUp(false)
	,mShouldSlowDown(false)
{
	auto vertices = getRectangleVertexBuffer();
	auto indices = getRectangleIndexBuffer();
	VertexBuffer vbo = createVertexBuffer(vertices.first, vertices.second);
	IndexBuffer ibo = createIndexBuffer(indices.first, indices.second);
	mVertexArray->setVertexBuffer(vbo, VertexBufferLayout::position2_texCoords2);
	mVertexArray->setIndexBuffer(ibo);

	mShader->loadFromFile("resources/shaders/basic.vs.glsl", "resources/shaders/basic.fs.glsl");
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

void Car::drawCurrent(const sf::Transform transform)
{
	mTexture.bind();
	Renderer::submit(mVertexArray, mShader, transform);
}

}
