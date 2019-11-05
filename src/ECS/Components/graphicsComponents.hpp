#pragma once

#include <SFML/Graphics/Color.hpp>

namespace ph{

class Texture;
class Shader;

namespace component {

	struct TextureRef
	{
		Texture* texture;
	};

	struct ShaderRef
	{
		Shader* shader;
	};

	struct Color
	{
		sf::Color color;
	};
}}