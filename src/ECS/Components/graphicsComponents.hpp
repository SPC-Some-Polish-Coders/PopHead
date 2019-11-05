#pragma once

#include <SFML/Graphics/Color.hpp>

namespace ph{

class Texture;
class Shader;

namespace component {

	struct TexturePtr
	{
		Texture* texture;
	};

	struct ShaderPtr
	{
		Shader* shader;
	};

	struct Color
	{
		sf::Color color;
	};
}}