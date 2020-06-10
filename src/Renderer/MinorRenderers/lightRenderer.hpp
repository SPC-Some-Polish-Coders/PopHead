#pragma once 
 
#include "Utilities/rect.hpp"
#include <SFML/Graphics/Color.hpp>

namespace ph::LightRenderer
{
	void init();
	void shutDown();

	void submitBunchOfLightWalls(const std::vector<FloatRect>&);
	void submitLightWall(FloatRect);
	void submitLight(sf::Color color, Vec2 position, float startAngle, float endAngle,
	                 float attenuationAddition, float attenuationFactor, float attenuationSquareFactor, bool rayCollisionDetection);

	void flush();

	u32 getNrOfCollisionLights();
	
	void setGameWorldCameraBoundsPtr(const FloatRect* cameraBounds);

	void submitDebug(sf::Color* ambientLightColor);
}

