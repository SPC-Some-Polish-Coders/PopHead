#pragma once

#include "Renderer/Buffers/indexBuffer.hpp"
#include "Utilities/rect.hpp"
#include "Utilities/vector4.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <optional>
#include <map>

namespace ph {

class Shader;
class Texture;
class VertexArray;

struct QuadData
{
	sf::Vector2f position;
	sf::Vector2f size;
	float rotation;
	Vector4f color;
	FloatRect textureRect;
	float textureSlotRef;
};

struct QuadRenderGroup
{
	std::vector<QuadData> quadsData;
	std::vector<const Texture*> textures;
};

class QuadRenderer
{
public:
	void init();
	void shutDown();

	void setScreenBoundsPtr(const FloatRect* screenBounds) { mScreenBounds = screenBounds; }

	unsigned getNumberOfDrawCalls() const { return mNumberOfDrawCalls; }
	unsigned getNumberOfDrawnSprites() const { return mNumberOfDrawnSprites; }
	unsigned getNumberOfDrawnTextures() const { return mNumberOfDrawnTextures; }
	unsigned getNumberOfRenderGroups() const { return mNumberOfRenderGroups; }

	void setDebugNumbersToZero();

	void submitQuad(const Texture*, const IntRect* textureRect, const sf::Color*,
	                sf::Vector2f position, sf::Vector2f size, float z, float rotation);
	void flush();

private:
	bool isInsideScreen(sf::Vector2f position, sf::Vector2f size, float rotation);
	auto getTextureSlotToWhichThisTextureIsBound(const Texture* texture, const QuadRenderGroup&) -> std::optional<float>;
	auto getNormalizedTextureRect(const IntRect* pixelTextureRect, sf::Vector2i textureSize) -> FloatRect;
	void bindTexturesForNextDrawCall(std::vector<const Texture*>& textures);
	void drawCall(unsigned nrOfInstances, std::vector<QuadData>& quadsData);

private:
	std::map<float, QuadRenderGroup, std::greater<>> mRenderGroups;
	const FloatRect* mScreenBounds;
	Shader* mDefaultInstanedSpriteShader;
	Texture* mWhiteTexture;
	IndexBuffer mQuadIBO;
	unsigned mQuadsDataVBO;
	unsigned mVAO;
	unsigned mNumberOfDrawCalls = 0;
	unsigned mNumberOfDrawnSprites = 0;
	unsigned mNumberOfDrawnTextures = 0;
	unsigned mNumberOfRenderGroups = 0;
};

}
