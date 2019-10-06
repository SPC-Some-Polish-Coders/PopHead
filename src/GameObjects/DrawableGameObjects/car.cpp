#include "car.hpp"
#include <array>
#include "Renderer/buffers.hpp"
#include "Renderer/renderer.hpp"

namespace
{
	std::array<float, 16> vertices = {
		// positions | texture coords
		0.5f,  0.5f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f  // top left 
	};

	std::array<unsigned, 6> indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
}

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
	VertexBuffer vbo = createVertexBuffer(vertices.data(), vertices.size() * sizeof(float));
	mVertexArray->setVertexBuffer(vbo, VertexBufferLayout::position2_texCoords2);
	IndexBuffer ibo = createIndexBuffer(indices.data(), indices.size() * sizeof(unsigned));
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
