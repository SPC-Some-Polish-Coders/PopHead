#pragma once

#include "quadData.hpp"
#include "Renderer/API/shader.hpp"
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

struct QuadRendererDebugArray
{
	unsigned data[100] = {};
	unsigned marker = 0;
};

struct QuadRendererDebugNumbers
{
	QuadRendererDebugArray renderGroupsSizes = {}; 
	QuadRendererDebugArray notAffectedByLightRenderGroupsSizes = {}; 
	unsigned renderGroups = 0;
	unsigned renderGroupsNotAffectedByLight = 0;
	unsigned drawCalls = 0;
	unsigned drawnSprites = 0;
	unsigned drawnTextures = 0;
};

class QuadRenderer
{
public:
	void init();
	void shutDown();

	QuadRendererDebugNumbers getDebugNumbers() { return mDebugNumbers; }

	void setScreenBoundsPtr(const FloatRect* bounds) { mScreenBounds = bounds; }
	void setDebugCountingActive(bool active) { mIsDebugCountingActive = active; }
	void resetDebugNumbers();

	void submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>&, const Texture*, const Shader*, float z, ProjectionType projectionType);

	void submitQuad(const Texture*, const IntRect* textureRect, const sf::Color*, const Shader*,
	                sf::Vector2f position, sf::Vector2f size, float z, float rotation, sf::Vector2f rotationOrigin, ProjectionType, bool isAffectedByLight);
	void flush(bool affectedByLight);

private:
	auto getTextureSlotToWhichThisTextureIsBound(const Texture* texture, const QuadRenderGroup&) -> std::optional<float>;
	auto getNormalizedTextureRect(const IntRect* pixelTextureRect, sf::Vector2i textureSize) -> FloatRect;
	void bindTexturesForNextDrawCall(std::vector<const Texture*>& textures);
	void drawCall(unsigned nrOfInstances, std::vector<QuadData>& quadsData);

private:
	RenderGroupsHashMap mRenderGroupsHashMap;
	RenderGroupsHashMap mNotAffectedByLightRenderGroupsHashMap;
	Shader mDefaultQuadShader;
	QuadRendererDebugNumbers mDebugNumbers;
	const FloatRect* mScreenBounds; 
	const Shader* mCurrentlyBoundQuadShader;
	Texture* mWhiteTexture;
	unsigned mQuadIBO;
	unsigned mQuadsDataVBO;
	unsigned mVAO;
	bool mIsDebugCountingActive = false;
};

}
