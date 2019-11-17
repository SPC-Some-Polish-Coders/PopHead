#pragma once

#include "Renderer/Vertices/indexBuffers.hpp"
#include "Utilities/rect.hpp"
#include "Utilities/vector4.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <optional>

namespace ph {

class Shader;
class Texture;
class VertexArray;

class QuadRenderer
{
public:
	void init();
	void shutDown();

	void setScreenBoundsPtr(const FloatRect*);
	void setViewProjectionMatrix(const float*);

	unsigned getNumberOfDrawCalls() const { return mNumberOfDrawCalls; }
	unsigned getNumberOfDrawnSprites() const { return mNumberOfDrawnSprites; }
	unsigned getNumberOfDrawnTextures() const { return mNumberOfDrawnTextures; }

	void setDebugNumbersToZero();

	void submitQuad(const Texture*, const IntRect* textureRect, const sf::Color*,
	                sf::Vector2f position, sf::Vector2f size, float rotation);
	void flush();

private:
	bool isInsideScreen(sf::Vector2f position, sf::Vector2f size);
	bool isInsideScreen(const FloatRect objectBounds);

	auto getTextureSlotToWhichThisTextureIsBound(const Texture* texture) -> std::optional<int>;
	auto getNormalizedTextureRect(const IntRect* pixelTextureRect, sf::Vector2f size) -> FloatRect;

private:
	std::vector<sf::Vector2f> mInstancedSpritesPositions;
	std::vector<sf::Vector2f> mInstancedSpritesSizes;
	std::vector<float> mInstancedSpritesRotations;
	std::vector<ph::Vector4f> mInstancedSpritesColors;
	std::vector<ph::FloatRect> mInstancedSpritesTextureRects;
	std::vector<int> mInstancedSpritesTextureSlotRefs;
	std::vector<const ph::Texture*> mInstancedTextures;

	const FloatRect* mScreenBounds;
	const float* mViewProjectionMatrix;
	
	IndexBuffer mQuadIBO;

	Shader* mDefaultInstanedSpriteShader;
	Texture* mWhiteTexture;

	unsigned mInstancedPositionsVBO;
	unsigned mInstancedSizesVBO;
	unsigned mInstancedRotationsVBO;
	unsigned mInstancedColorsVBO;
	unsigned mInstancedTextureRectsVBO;
	unsigned mInstancedVAO;

	const int mNrOfSpritesInOneInstancedDrawCall = 2500;

	unsigned mNumberOfDrawCalls = 0;
	unsigned mNumberOfDrawnSprites = 0;
	unsigned mNumberOfDrawnTextures = 0;
};

}
