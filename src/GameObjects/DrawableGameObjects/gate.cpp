#include "gate.hpp"
#include "Physics/physicsEngine.hpp"

namespace ph{

Gate::Gate(const Texture& texture, const sf::Vector2f position, PhysicsEngine& physicsEngine, const bool isOpen)
	:GameObject("gate")
	,mVertexArray(std::make_shared<VertexArray>())
	,mShader(std::make_shared<Shader>())
	,mTexture(texture)
	,mCollisionBody(physicsEngine.createStaticBodyAndGetTheReference(sf::FloatRect(position.x, position.y + 6, 32, 10)))
	,mPhysicsEngine(physicsEngine)
	,mIsOpen(isOpen)
{
	setPosition(position);

	auto vbo = VertexBufferHolder::getGlobalInstance().getRectangleVertexBuffer("gate", texture.getWidth(), texture.getHeight());
	auto ibo = IndexBufferHolder::getGlobalInstance().getRectangleIndexBuffer("rectangle");
	mVertexArray->setVertexBuffer(vbo, VertexBufferLayout::position2_texCoords2);
	mVertexArray->setIndexBuffer(ibo);

	mShader->loadFromFile("resources/shaders/basic.vs.glsl", "resources/shaders/basic.fs.glsl");
}

void Gate::drawCurrent(sf::Transform transform)
{
	if(!mIsOpen) {
		mTexture.bind();
		Renderer::submit(mVertexArray, mShader, transform);
	}
}

void Gate::open()
{
	if(mIsOpen)
		return;
	mPhysicsEngine.removeStaticBody(mCollisionBody);
	mIsOpen = true;
}

void Gate::close()
{
	if(!mIsOpen)
		return;
	// TODO: Fix bug that gate can be closed only one time, Fixing this requires changes in PhysicsEngine
	mPhysicsEngine.createStaticBodyAndGetTheReference(sf::FloatRect(getPosition().x, getPosition().y + 6, 32, 10));
	mIsOpen = false;
}

}
