#include "slowQuadRenderer.hpp"
#include "Renderer/Shaders/shaderLibary.hpp"
#include "Renderer/Vertices/vertexArray.hpp"
#include "Renderer/texture.hpp"
#include "Renderer/openglErrors.hpp"
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
	IndexBuffer quadIBO = createIndexBuffer();
	setData(quadIBO, quadIndices, sizeof(quadIndices));

	VertexBuffer textureQuadVBO = createVertexBuffer();
	setData(textureQuadVBO, quadPositionsAndTextureCoords, sizeof(quadPositionsAndTextureCoords), DataUsage::staticDraw);
	mTextureQuadVertexArray = new VertexArray;
	mTextureQuadVertexArray->setVertexBuffer(textureQuadVBO, VertexBufferLayout::position2_texCoords2);
	mTextureQuadVertexArray->setIndexBuffer(quadIBO);

	VertexBuffer animatedTextureQuadVBO = createVertexBuffer();
	setData(animatedTextureQuadVBO, nullptr, sizeof(quadPositionsAndTextureCoords), DataUsage::dynamicDraw);
	mTextureAnimatedQuadVertexArray = new VertexArray;
	mTextureAnimatedQuadVertexArray->setVertexBuffer(animatedTextureQuadVBO, VertexBufferLayout::position2_texCoords2);
	mTextureAnimatedQuadVertexArray->setIndexBuffer(quadIBO);

	mWhiteTexture = new Texture;
	unsigned whiteData = 0xffffffff;
	mWhiteTexture->setData(&whiteData, sizeof(unsigned), sf::Vector2i(1, 1));
}

void SlowQuadRenderer::reset()
{
	init();
	shutDown();
}

void SlowQuadRenderer::shutDown()
{
	delete mTextureQuadVertexArray;
	delete mTextureAnimatedQuadVertexArray;
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
	if(!isInsideScreen(position, size))
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
			setTextureRect(mTextureAnimatedQuadVertexArray->getVertexBuffer(), *textureRect, texture->getSize());
			mTextureAnimatedQuadVertexArray->bind();
		}
		else
			mTextureQuadVertexArray->bind();

		texture->bind();
	}
	else {
		mTextureQuadVertexArray->bind();
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
	// TODO_ren: Does viewProjectionMatrix have to be set for each object even if we don't change shader
}

bool SlowQuadRenderer::isInsideScreen(sf::Vector2f position, sf::Vector2i size)
{
	return isInsideScreen(sf::FloatRect(position.x, position.y, static_cast<float>(size.x), static_cast<float>(size.y)));
}

bool SlowQuadRenderer::isInsideScreen(const FloatRect objectBounds)
{
	return mScreenBounds->doPositiveRectsIntersect(objectBounds);
}

}
