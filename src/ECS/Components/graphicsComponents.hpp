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
	};

	struct TextureRect
	{
		IntRect rect;
	};

	struct GroundRenderChunk
	{
		FloatRect bounds;
		FloatRect textureRect;
		unsigned char z;
	};

	struct RenderChunk
	{
		std::vector<ChunkQuadData> quads;
		std::vector<FloatRect> lightWalls;
		FloatRect quadsBounds;
		FloatRect lightWallsBounds;
		unsigned char z;
		unsigned rendererID;
	};

	struct LightWall
	{
		FloatRect rect;
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

	struct Camera
	{
		ph::Camera camera;
		std::string name;

		inline static std::string currentCameraName;
	};

	struct CameraShake
	{
		float duration;
		float elapsedTime = 0.f;
		float magnitude;
		bool smooth = false;
	};

	struct DebugCamera
	{
	};

	struct HiddenForRenderer
	{
	};

}}
