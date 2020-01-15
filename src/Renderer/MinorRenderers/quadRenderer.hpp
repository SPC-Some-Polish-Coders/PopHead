#pragma once

#include "quadData.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/API/indexBuffer.hpp"
#include "Utilities/rect.hpp"
#include "Utilities/vector4.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <optional>
#include <map>
#include <utility>

namespace ph {

class Texture;

bool operator == (const RenderGroupKey& lhs, const RenderGroupKey& rhs);

class RenderGroupsHashMap
{
public:
	RenderGroupsHashMap();
	QuadRenderGroup& insertIfDoesNotExistAndGetRenderGroup(RenderGroupKey);
	auto getUnderlyingVector() -> std::vector<std::pair<RenderGroupKey, QuadRenderGroup>>&;
	size_t size() const { return mRenderGroups.size(); }
private:
	QuadRenderGroup* getRenderGroup(RenderGroupKey);
	void sort();
	void eraseUselessGroups();

private:
	std::vector<std::pair<RenderGroupKey, QuadRenderGroup>> mRenderGroups;
	bool mShouldSort;
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

	void setDebugCountingActive(bool active) { mIsDebugCountingActive = active; }
	void setDebugNumbersToZero();

	void submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>&, const Texture*, const Shader*, float z, ProjectionType projectionType);

	void submitQuad(const Texture*, const IntRect* textureRect, const sf::Color*, const Shader*,
	                sf::Vector2f position, sf::Vector2f size, float z, float rotation, sf::Vector2f rotationOrigin, ProjectionType);
	void flush();

private:
	bool isInsideScreen(sf::Vector2f position, sf::Vector2f size, float rotation);
	auto getTextureSlotToWhichThisTextureIsBound(const Texture* texture, const QuadRenderGroup&) -> std::optional<float>;
	auto getNormalizedTextureRect(const IntRect* pixelTextureRect, sf::Vector2i textureSize) -> FloatRect;
	void bindTexturesForNextDrawCall(std::vector<const Texture*>& textures);
	void drawCall(unsigned nrOfInstances, std::vector<QuadData>& quadsData);

private:
	RenderGroupsHashMap mRenderGroupsHashMap;
	Shader mDefaultQuadShader;
	const FloatRect* mScreenBounds;
	const Shader* mCurrentlyBoundQuadShader;
	Texture* mWhiteTexture;
	IndexBuffer mQuadIBO;
	unsigned mQuadsDataVBO;
	unsigned mVAO;
	unsigned mNumberOfDrawCalls = 0;
	unsigned mNumberOfDrawnSprites = 0;
	unsigned mNumberOfDrawnTextures = 0;
	unsigned mNumberOfRenderGroups = 0;
	bool mIsDebugCountingActive = false;
};

}
