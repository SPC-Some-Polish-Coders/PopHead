#pragma once

#include "quadData.hpp"
#include "Renderer/API/shader.hpp"
#include "Utilities/rect.hpp"
#include "Utilities/vector4.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <optional>

namespace ph {

class Texture;

/*
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

RenderGroupsHashMap::RenderGroupsHashMap()
	:mShouldSort(false)
{
	mRenderGroups.reserve(10);
}

QuadRenderGroup& RenderGroupsHashMap::insertIfDoesNotExistAndGetRenderGroup(RenderGroupKey key) 
{
	if(auto renderGroup = getRenderGroup(key))
		return *renderGroup;
	mRenderGroups.emplace_back(std::pair(key, QuadRenderGroup()));
	mShouldSort = true;
	return *getRenderGroup(key);
}

auto RenderGroupsHashMap::getUnderlyingVector() -> std::vector<std::pair<RenderGroupKey, QuadRenderGroup>>&
{
	eraseUselessGroups();
	sort();
	return mRenderGroups;
}

void RenderGroupsHashMap::sort()
{
	if(!mShouldSort)
		return;

	// TODO_ren: Make more smart sorting so we don't need to rebind shaders that often
	//           Use the fact that we don't need to sort everything by z because not every quad is transparent

	std::sort(mRenderGroups.begin(), mRenderGroups.end(), []
	(const std::pair<RenderGroupKey, QuadRenderGroup>& a, std::pair<RenderGroupKey, QuadRenderGroup>& b) {
		return a.first.z > b.first.z;
	});
}

void RenderGroupsHashMap::eraseUselessGroups()
{
	for(size_t i = 0; i < mRenderGroups.size(); ++i)
		if(mRenderGroups[i].second.quadsData.empty())
			mRenderGroups.erase(mRenderGroups.begin() + i);
}

QuadRenderGroup* RenderGroupsHashMap::getRenderGroup(RenderGroupKey key)
{
	for(size_t i = 0; i < mRenderGroups.size(); ++i)
		if(mRenderGroups[i].first == key)
			return &mRenderGroups[i].second;
	return nullptr;
}

bool operator==(const RenderGroupKey& lhs, const RenderGroupKey& rhs)
{
	return lhs.shader == rhs.shader && lhs.z == rhs.z;
}*/

struct QuadRendererDebugArray
{
	unsigned data[100] = {};
	unsigned marker = 0;
};

struct QuadRendererDebugNumbers
{
	QuadRendererDebugArray renderGroupsSizes = {}; 
	QuadRendererDebugArray renderGroupsZ = {}; 
	QuadRendererDebugArray renderGroupsIndices = {}; 
	QuadRendererDebugArray notAffectedByLightRenderGroupsSizes = {}; 
	QuadRendererDebugArray notAffectedByLightRenderGroupsZ = {}; 
	QuadRendererDebugArray notAffectedByLightRenderGroupsIndices = {}; 
	unsigned allocations = 0;
	unsigned renderGroups = 0;
	unsigned renderGroupsNotAffectedByLight = 0;
	unsigned drawCalls = 0;
	unsigned drawnSprites = 0;
	unsigned drawnTextures = 0;
};

QuadRendererDebugNumbers getQuadRendererDebugNumbers();
void resetQuadRendererDebugNumbers();

class QuadRenderer
{
public:
	void init();
	void shutDown();

	void setScreenBoundsPtr(const FloatRect* bounds) { mScreenBounds = bounds; }
	void setDebugCountingActive(bool active) { mIsDebugCountingActive = active; }

	void submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>&, Texture*, const Shader*, float z, ProjectionType projectionType);

	void submitQuad(Texture*, const IntRect* textureRect, const sf::Color*, const Shader*,
	                sf::Vector2f position, sf::Vector2f size, float z, float rotation, sf::Vector2f rotationOrigin, ProjectionType, bool isAffectedByLight);
	void flush(bool affectedByLight);

private:
	Shader mDefaultQuadShader;
	const FloatRect* mScreenBounds; 
	const Shader* mCurrentlyBoundQuadShader;
	Texture* mWhiteTexture;
	unsigned mQuadIBO;
	unsigned mQuadsDataVBO;
	unsigned mVAO;
	bool mIsDebugCountingActive = false;
};

}
