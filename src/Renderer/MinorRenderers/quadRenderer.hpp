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
	unsigned chunks = 0;
	unsigned cachedChunks = 0;
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

	void submitGroundChunk(sf::Vector2f pos, const Texture&, const FloatRect& textureRect, float z);

	void submitChunk(std::vector<ChunkQuadData>& quadsData, const Texture& texture,
	                 const FloatRect& bounds, float z, unsigned* rendererID);

	void submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>&, Texture*, const Shader*, float z, ProjectionType projectionType);

	void submitQuad(Texture*, const IntRect* textureRect, const sf::Color*, const Shader*,
	                sf::Vector2f position, sf::Vector2f size, float z, float rotation, sf::Vector2f rotationOrigin, ProjectionType, bool isAffectedByLight);
	void flush(bool affectedByLight);

private:
	Shader mDefaultQuadShader;
	Shader mGroundChunkShader;
	Shader mChunkShader;
	const FloatRect* mScreenBounds; 
	Texture* mWhiteTexture;
	unsigned mQuadIBO;
	unsigned mQuadsDataVBO;
	unsigned mVAO;
	bool mIsDebugCountingActive = false;
};

}
