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
		using IntRect::operator=;
	};

	struct IndoorOutdoorBlendArea
	{
		enum ExitSide {Left, Right, Top, Down};
		ExitSide exit;
	};

	struct IndoorOutdoorBlend
	{
		float outdoor;
		float brightness;
		float alpha;
	};

	struct OutdoorBlend
	{
		float brightness;
	};

	struct IndoorBlend
	{
		float alpha;
	};

	struct RenderQuad
	{
		Texture* texture;
		Shader* shader;
		Vec2 rotationOrigin;
		sf::Color color;
		float rotation;
		u8 z;
	};

	struct GroundRenderChunk
	{
		FloatRect bounds;
		FloatRect textureRect;
		u8 z;
		bool outdoor;
	};

	struct RenderChunk
	{
		std::vector<ChunkQuadData> quads;
		std::vector<FloatRect> lightWalls;
		FloatRect quadsBounds;
		FloatRect lightWallsBounds;
		u8 z;
		u32 rendererID;
		bool outdoor;
	};

	struct LightWall : public FloatRect 
	{
		using FloatRect::operator=;
	};

	struct LightSource
	{
		Vec2 offset;
		sf::Color color;
		float attenuationAddition;
		float attenuationFactor;
		float attenuationSquareFactor;
		float startAngle;
		float endAngle;
	};

	struct CameraRoom 
	{
		float timeFromPlayerEntrance = 0.f;
		float edgeAreaSize = 0.f; // from 0 to 1
		bool playerWasInCenter;
	};

	struct Camera : public ph::Camera
	{
		std::string name;
		inline static std::string currentCameraName;

		using ph::Camera::operator=;
	};

	struct CameraShake // @no-debugger
	{
		float duration;
		float elapsedTime = 0.f;
		float magnitude;
		bool smooth = false;
	};

	struct DebugCamera {}; // @no-debugger

	struct HiddenForRenderer {};

}}
