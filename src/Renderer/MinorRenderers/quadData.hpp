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
	Vector4f color;
	FloatRect textureRect;
	sf::Vector2f position;
	sf::Vector2f size;
	sf::Vector2f rotationOrigin;
	float rotation;
	float textureSlotRef;
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
	std::vector<QuadData> quadsData;
	std::vector<const Texture*> textures;
};

}
