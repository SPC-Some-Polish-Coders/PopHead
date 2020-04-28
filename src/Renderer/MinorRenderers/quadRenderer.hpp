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

void setChunksTexture(unsigned texture);

void submitGroundChunk(sf::Vector2f pos, const FloatRect& textureRect, float z);

unsigned registerNewChunk(const FloatRect& bounds);

void submitChunk(std::vector<ChunkQuadData>& quadsData,
				 const FloatRect& bounds, float z, unsigned* rendererID);

void submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>&, Texture*, const Shader*, float z, ProjectionType projectionType);

void submitQuad(Texture*, const IntRect* textureRect, const sf::Color*, const Shader*,
				sf::Vector2f position, sf::Vector2f size, float z, float rotation, sf::Vector2f rotationOrigin, ProjectionType, bool isAffectedByLight);
void flush(bool affectedByLight);

}

