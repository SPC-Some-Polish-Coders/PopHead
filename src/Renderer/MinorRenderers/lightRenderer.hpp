#pragma once 
 
#include "Renderer/API/shader.hpp"
#include "quadData.hpp"
#include <SFML/Graphics/Color.hpp>
#include "Utilities/rect.hpp"
#include <vector>
#include <optional>

namespace ph { 

// TODO: Add Z to Light

struct Light
{
	sf::Color color;
	sf::Vector2f pos;
	float startAngle;
	float endAngle;
	float attenuationAddition;
	float attenuationFactor;
	float attenuationSquareFactor;
};

struct Ray
{
	sf::Vector2f direction;
	float angle;
};

struct Wall
{
	sf::Vector2f point1;
	sf::Vector2f point2;
};

struct RayWallIntersection
{
	sf::Vector2f point;
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
	void submitLight(Light);
	unsigned getNrOfLights(); 

	void submitDebug();

	void flush();
	
	void setScreenBoundsPtr(const FloatRect* screenBounds) { mScreenBounds = screenBounds; }

private:
	RayWallIntersection getRayWallClosestIntersection(sf::Vector2f rayDir, sf::Vector2f lightPos, FloatRect wall);
	sf::Vector2f getVectorLineIntersectionPoint(sf::Vector2f rayDir, sf::Vector2f lightPos, sf::Vector2f lineP1, sf::Vector2f lineP2);

private:
	std::vector<FloatRect> mLightWalls;
	std::vector<Light> mLights;
	std::vector<sf::Vector2f> mLightTriangleFanVertexData;
	const FloatRect* mScreenBounds;
	Shader mLightShader;
	unsigned mLightTriangleFanVAO, mLightTriangleFanVBO;
};

} 
