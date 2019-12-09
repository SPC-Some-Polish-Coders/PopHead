#pragma once 
 
#include <SFML/Graphics/Color.hpp>
#include "Utilities/rect.hpp"
#include <vector>
#include <optional>

namespace ph { 

struct Light
{
	sf::Color color;
	sf::Vector2f position;
	float startAngle;
	float endAngle;
	float range;
};

struct RayDestinationPoint
{
	sf::Vector2f position;
	unsigned leftWallID;
	unsigned rightWallID;
};

struct Ray
{
	sf::Vector2f position;
	sf::Vector2f direction;
};

struct Wall
{
	sf::Vector2f leftPointPosition;
	sf::Vector2f rightPointPosition;
};

class LightRenderer
{
public:
	void submitLightBlockingQuad(sf::Vector2f position, sf::Vector2f size);
	void submitLight(Light);
	void flush();

private:
	auto getPointOfIntersection(const Ray&, const Wall&) -> std::optional<sf::Vector2f>;

private:
	std::vector<RayDestinationPoint> mWallPoints;
	std::vector<Wall> mWalls;
	std::vector<Light> mLights;
	std::vector<Ray> mRays;
};

// TODO_ren: Add submitLine()

} 
