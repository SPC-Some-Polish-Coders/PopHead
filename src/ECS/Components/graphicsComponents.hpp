#pragma once

#include <SFML/Graphics/Color.hpp>
#include "Renderer/API/camera.hpp"
#include "Renderer/MinorRenderers/quadData.hpp"
#include <vector>

namespace ph{

class Texture;
class Shader;

namespace component {

	struct RenderQuad
	{
		Texture* texture;
		Shader* shader;
		sf::Vector2f rotationOrigin;
		sf::Color color;
		float rotation;
		unsigned char z;
		bool blocksLight;
	};

	struct TextureRect
	{
		IntRect rect;
	};

	struct RenderChunk
	{
		std::vector<QuadData> quads;
		FloatRect bounds;
		unsigned char z;
	};

	struct Camera
	{
		ph::Camera camera;
		unsigned priority; // greater number - greater priority, lesser number - lesser priority
	};

	struct LightSource
	{
		sf::Vector2f offset;
		sf::Color color;
		float attenuationAddition;
		float attenuationFactor;
		float attenuationSquareFactor;
		float startAngle;
		float endAngle;
	};

	struct HiddenForRenderer
	{
	};

}}
