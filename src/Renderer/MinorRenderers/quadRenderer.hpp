#pragma once

#include "quadData.hpp"
#include "Renderer/API/shader.hpp"
#include "Utilities/rect.hpp"
#include "Utilities/vector4.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <optional>
#include <vector>

namespace ph {
	class Texture;
}

namespace ph::QuadRenderer {

void init();
void shutDown();

void setScreenBoundsPtr(const FloatRect* bounds);
void setDebugCountingActive(bool active);

void setChunksTexture(u32 texture);

void submitGroundChunk(Vec2 pos, const FloatRect& textureRect, float z, sf::Color color);

u32 registerNewChunk(const FloatRect& bounds);

void submitChunk(std::vector<ChunkQuadData>& quadsData,
				 const FloatRect& bounds, float z, u32* rendererID, sf::Color color);

void submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>&, Texture*, const Shader*, float z, ProjectionType projectionType);

void submitQuad(Texture*, const IntRect* textureRect, const sf::Color*, const Shader*,
				Vec2 position, Vec2 size, float z, float rotation, Vec2 rotationOrigin, ProjectionType, bool isAffectedByLight);
void flush(bool affectedByLight);

void submitDebug();

}

