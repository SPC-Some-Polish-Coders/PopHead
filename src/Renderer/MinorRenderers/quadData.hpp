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
	sf::Vector2f position;
	sf::Vector2f size;
	float rotation;
	Vector4f color;
	FloatRect textureRect;
	float textureSlotRef;
};

struct RenderGroupKey
{
	const Shader* shader;
	float z;
};

struct QuadRenderGroup
{
	std::vector<QuadData> quadsData;
	std::vector<const Texture*> textures;
};

}
