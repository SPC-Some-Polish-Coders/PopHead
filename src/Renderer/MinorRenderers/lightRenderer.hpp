#pragma once 
 
#include "Renderer/API/shader.hpp"
#include "quadData.hpp"
#include "Utilities/rect.hpp"
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <optional>

namespace ph { 

// TODO: Add Z to Light

struct Light
{
	sf::Color color;
	Vec2 pos;
	float startAngle;
	float endAngle;
	float attenuationAddition;
	float attenuationFactor;
	float attenuationSquareFactor;
};

struct Ray
{
	Vec2 direction;
	float angle;
};

struct Wall
{
	Vec2 point1;
	Vec2 point2;
};

struct RayWallIntersection
{
	Vec2 point;
	float distance;
	bool valid = false;
};

class LightRenderer
{
public:
	void init();
	void shutDown();

	void clearStaticLightWalls();
	void submitBunchOfLightWalls(const std::vector<FloatRect>&);
	void submitLightWall(FloatRect);
	void submitLight(Light, bool rayCollisionDetection);

	void flush();
	
	void setScreenBoundsPtr(const FloatRect* screenBounds) { mScreenBounds = screenBounds; }

	void submitDebug(sf::Color* ambientLightColor);
	u32 getNrOfLights(); 

private:
	RayWallIntersection getRayWallClosestIntersection(Vec2 rayDir, Vec2 lightPos, FloatRect wall);
	Vec2 getVectorLineIntersectionPoint(Vec2 rayDir, Vec2 lightPos, Vec2 lineP1, Vec2 lineP2);

private:
	std::vector<FloatRect> mLightWalls;
	std::vector<Light> mLights;
	std::vector<Light> mNoCollisionLights;
	std::vector<Vec2> mLightTriangleFanVertexData;
	const FloatRect* mScreenBounds;
	Shader mLightShader;
	Shader mNoCollisionLightShader;
	u32 mLightTriangleFanVao, mLightTriangleFanVbo;
};

} 
