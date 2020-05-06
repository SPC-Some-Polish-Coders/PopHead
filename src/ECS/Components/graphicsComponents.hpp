#pragma once

#include <SFML/Graphics/Color.hpp>
#include "Renderer/API/camera.hpp"
#include "Renderer/MinorRenderers/quadData.hpp"
#include "Utilities/rect.hpp"
#include <vector>

namespace ph{

class Texture;
class Shader;

namespace component {

	struct TextureRect : public IntRect 
	{
		operator IntRect&() { return *this; }
		TextureRect& operator=(const IntRect& r) { pos = r.pos; size = r.size; return *this; }
	};

	struct IndoorOutdoorBlendArea
	{
		enum ExitSide {Left, Right, Top, Down};
		ExitSide exit;
	};

	struct IndoorOutdoorBlend
	{
		float outdoor;
		float outdoorDarkness;
		float indoorAlpha;
	};

	struct OutdoorBlend
	{
		float darkness;
	};

	struct IndoorBlend
	{
		float alpha;
	};

	struct RenderQuad
	{
		Texture* texture;
		Shader* shader;
		sf::Vector2f rotationOrigin;
		sf::Color color;
		float rotation;
		unsigned char z;
	};

	struct GroundRenderChunk
	{
		FloatRect bounds;
		FloatRect textureRect;
		unsigned char z;
		bool outdoor;
	};

	struct RenderChunk
	{
		std::vector<ChunkQuadData> quads;
		std::vector<FloatRect> lightWalls;
		FloatRect quadsBounds;
		FloatRect lightWallsBounds;
		unsigned char z;
		unsigned rendererID;
		bool outdoor;
	};

	struct LightWall : public FloatRect 
	{
		operator FloatRect&() { return *this; }
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

	struct DebugCamera {};

	struct HiddenForRenderer {};

}}
