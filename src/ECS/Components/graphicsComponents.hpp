#pragma once

#include <SFML/Graphics/Color.hpp>
#include "Renderer/camera.hpp"
#include <vector>

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

	struct Z
	{
		char z;
	};

	struct Camera
	{
		ph::Camera camera;
		unsigned priority; // greater number - greater priority, lesser number - lesser priority
	};

	struct HiddenForRenderer
	{
	};
}}
