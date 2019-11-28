#include "slowQuadRenderer.hpp"
#include "Renderer/Shaders/shaderLibary.hpp"
#include "Renderer/Buffers/vertexArray.hpp"
#include "Renderer/texture.hpp"
#include "Renderer/glEnums.hpp"
#include "Renderer/openglErrors.hpp"
#include "Utilities/profiling.hpp"
#include <SFML/Graphics/Transform.hpp>
#include <GL/glew.h>

namespace ph {

void SlowQuadRenderer::init()
{
	// load default shader
	auto& sl = ShaderLibrary::getInstance();
	sl.loadFromFile("sprite", "resources/shaders/sprite.vs.glsl", "resources/shaders/sprite.fs.glsl");
	mDefaultSpriteShader = sl.get("sprite");

	// set up vertex array
	float quadPositionsAndTextureCoords[] = {
		1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	unsigned quadIndices[] = {0, 1, 3, 1, 2, 3};
	mQuadIBO.init();
	mQuadIBO.setData(quadIndices, sizeof(quadIndices));

	mTextureQuadVBO.init();
	mTextureQuadVBO.setData(quadPositionsAndTextureCoords, sizeof(quadPositionsAndTextureCoords), DataUsage::staticDraw);
	mTextureQuadVertexArray.init();
	mTextureQuadVertexArray.setVertexBuffer(mTextureQuadVBO, VertexBufferLayout::position2_texCoords2);
	mTextureQuadVertexArray.setIndexBuffer(mQuadIBO);

	mAnimatedTextureQuadVBO.init();
	mAnimatedTextureQuadVBO.setData(nullptr, sizeof(quadPositionsAndTextureCoords), DataUsage::dynamicDraw);
	mTextureAnimatedQuadVertexArray.init();
	mTextureAnimatedQuadVertexArray.setVertexBuffer(mAnimatedTextureQuadVBO, VertexBufferLayout::position2_texCoords2);
	mTextureAnimatedQuadVertexArray.setIndexBuffer(mQuadIBO);

	mWhiteTexture = new Texture;
	unsigned whiteData = 0xffffffff;
	mWhiteTexture->setData(&whiteData, sizeof(unsigned), sf::Vector2i(1, 1));
}

void SlowQuadRenderer::shutDown()
{
	mTextureQuadVertexArray.remove();
	mTextureAnimatedQuadVertexArray.remove();
	mAnimatedTextureQuadVBO.remove();
	delete mWhiteTexture;
}

void SlowQuadRenderer::setScreenBoundsPtr(const FloatRect* screenBounds)
{
	mScreenBounds = screenBounds;
}

void SlowQuadRenderer::setViewProjectionMatrix(const float* viewProjectionMatrix)
{
	mViewProjectionMatrix = viewProjectionMatrix;
}

void SlowQuadRenderer::drawQuad(const Texture* texture, const IntRect* textureRect, const sf::Color* color, const Shader* shader,
                                sf::Vector2f position, sf::Vector2i size, float rotation)
{
	// culling
	if(!isInsideScreen(position, size, rotation))
		return;

	// shader
	if(!shader)
		shader = mDefaultSpriteShader;
	shader->bind();

	if(!color)
		shader->setUniformVector4Color("color", sf::Color::White);
	else
		shader->setUniformVector4Color("color", *color);
	setQuadTransformUniforms(shader, position, size, rotation);

	// texture
	if(texture) {
		if(textureRect) {
			mTextureAnimatedQuadVertexArray.getVertexBuffer().setTextureRect(*textureRect, texture->getSize());
			mTextureAnimatedQuadVertexArray.bind();
		}
		else
			mTextureQuadVertexArray.bind();

		texture->bind();
	}
	else {
		mTextureQuadVertexArray.bind();
		mWhiteTexture->bind();
	}

	// draw call
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );

	// set debug data
	++mNumberOfDrawCalls;
}

void SlowQuadRenderer::setQuadTransformUniforms(const Shader* shader, sf::Vector2f position, const sf::Vector2i size, float rotation)
{
	sf::Transform transform;
	transform.translate(position);
	transform.scale(static_cast<sf::Vector2f>(size));
	if(rotation != 0.f)
		transform.rotate(rotation);
	shader->setUniformMatrix4x4("modelMatrix", transform.getMatrix());
	shader->setUniformMatrix4x4("viewProjectionMatrix", mViewProjectionMatrix);
}

bool SlowQuadRenderer::isInsideScreen(sf::Vector2f pos, sf::Vector2i size, float rotation)
{
	if(rotation == 0.f)
		return isInsideScreen(sf::FloatRect(pos.x, pos.y, static_cast<float>(size.x), static_cast<float>(size.y)));
	else
		return isInsideScreen(sf::FloatRect(pos.x - size.x * 2, pos.y - size.x * 2, static_cast<float>(size.x * 4), static_cast<float>(size.y * 4)));
}

bool SlowQuadRenderer::isInsideScreen(const FloatRect objectBounds)
{
	return mScreenBounds->doPositiveRectsIntersect(objectBounds);
}

}
