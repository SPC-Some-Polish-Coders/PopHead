#pragma once

#include <SFML/System/Vector2.hpp>
#include "Utilities/vector4.hpp"
#include "Utilities/rect.hpp"
#include <vector>

namespace ph {

class Shader;
class Texture;

struct QuadData
{
	Vec4 color;
	FloatRect textureRect;
	Vec2 position;
	Vec2 size;
	Vec2 rotationOrigin;
	float rotation;
	float textureSlotRef;
};

struct ChunkQuadData
{
	FloatRect textureRect;
	Vec2 position;
	Vec2 size;
	float rotation;
};

enum class ProjectionType{ gameWorld, gui };

struct RenderGroupKey
{
	const Shader* shader;
	float z;
	ProjectionType projectionType;
};

struct QuadRenderGroup
{
	u32* textures;

	QuadData* quadsData;

	u32 texturesSize;
	u32 texturesCapacity;

	u32 quadsDataArenaSize;
	u32 quadsDataSize;
	u32 quadsDataCapacity;
};

}
