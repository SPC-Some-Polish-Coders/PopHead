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

	struct TextureRect
	{
		IntRect rect;
	};

	struct ShaderPtr
	{
		Shader* shader;
	};

	struct Color
	{
		sf::Color color;
	};

	struct Rotation
	{
		float angle;
	};
}}