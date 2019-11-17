#pragma once

#include "Utilities/rect.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Color.hpp"

namespace ph {

class Texture;
class Shader;
class VertexArray;

class SlowQuadRenderer
{
public:
	void init();
	void reset();
	void shutDown();

	void setScreenBoundsPtr(const FloatRect*);

	void setViewProjectionMatrix(const float*);

	void drawQuad(const Texture*, const IntRect*, const sf::Color*, const Shader*,
	              sf::Vector2f position, sf::Vector2i size, float rotation = 0.f);

private:
	void setQuadTransformUniforms(const Shader* shader, sf::Vector2f position, const sf::Vector2i size, float rotation);
	bool isInsideScreen(sf::Vector2f position, sf::Vector2i size);
	bool isInsideScreen(const FloatRect objectBounds);

private:
	VertexArray* mTextureQuadVertexArray;
	VertexArray* mTextureAnimatedQuadVertexArray;
	Shader* mDefaultSpriteShader;
	Texture* mWhiteTexture;
	const FloatRect* mScreenBounds;
	const float* mViewProjectionMatrix;
	int mNumberOfDrawCalls;
};

}