#pragma once 
 
#include "Renderer/API/shader.hpp"
#include <SFML/Graphics/Color.hpp>
#include "Utilities/rect.hpp"
#include <vector>
#include <optional>

namespace ph { 

struct LightingDebug
{
	bool drawLight = true;
	bool drawWalls = false;
	bool drawRays = false;
};

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

// TODO_ren: Add submit light blocking line

class LightRenderer
{
public:
	void init();
	void shutDown();

	void submitLightBlockingQuad(sf::Vector2f position, sf::Vector2f size);
	void submitLight(Light);
	void flush();
	
	void setScreenBoundsPtr(const FloatRect* screenBounds) { mScreenBounds = screenBounds; }

	static LightingDebug& getDebug() { return sDebug; }

private:
	auto getIntersectionPoint(const sf::Vector2f rayDir, sf::Vector2f lightPos, const Wall& wall) -> std::optional<sf::Vector2f>;

private:
	std::vector<Wall> mWalls;
	std::vector<Light> mLights;
	std::vector<sf::Vector2f> mLightPolygonVertexData;
	const FloatRect* mScreenBounds;
	Shader mLightShader;
	unsigned mVAO, mVBO;

	inline static LightingDebug sDebug;
};

} 
