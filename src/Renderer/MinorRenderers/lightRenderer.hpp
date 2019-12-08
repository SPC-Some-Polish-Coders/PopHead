#pragma once 
 
#include <SFML/Graphics/Color.hpp>
#include "Utilities/rect.hpp"
#include <vector>

namespace ph { 
 
class PointRenderer;

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
	int wallID1;
	int wallID2;
};

class LightRenderer
{
public:
	void submitWallQuad(sf::Vector2f position, sf::Vector2f size);
	void submitLight(Light);
	void flush(PointRenderer& pointRenderer);

private:
	std::vector<RayDestinationPoint> mWallPoints;
	std::vector<Light> mLights;
};

} 
