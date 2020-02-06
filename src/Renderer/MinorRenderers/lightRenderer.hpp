#pragma once 
 
#include "Renderer/API/shader.hpp"
#include "quadData.hpp"
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

struct LocalIllumination
{
	sf::Vector2f pos;
	sf::Vector2f size;
	ProjectionType projectionType;
	sf::Color color;
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
	void submitLocalIllumination(LocalIllumination);

	void flush();
	
	void setScreenBoundsPtr(const FloatRect* screenBounds) { mScreenBounds = screenBounds; }

	static LightingDebug& getDebug() { return sDebug; }

private:
	auto getIntersectionPoint(const sf::Vector2f rayDir, sf::Vector2f lightPos, const Wall& wall) -> std::optional<sf::Vector2f>;

private:
	std::vector<Wall> mWalls;
	std::vector<Light> mLights;
	std::vector<sf::Vector2f> mLightTriangleFanVertexData;
	std::vector<LocalIllumination> mLocalIlluminations;
	const FloatRect* mScreenBounds;
	Shader mLightShader;
	Shader mLocalIlluminationShader;
	unsigned mLightTriangleFanVAO, mLightTriangleFanVBO;
	unsigned mLocalIlluminationQuadVAO, mLocalIlluminationQuadVBO, mLocalIlluminationQuadIBO;

	inline static LightingDebug sDebug;
};

} 
