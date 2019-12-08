#include "lightRenderer.hpp"
#include "pointRenderer.hpp"

namespace ph { 

void LightRenderer::submitWallQuad(sf::Vector2f position, sf::Vector2f size)
{
	mWallPoints.emplace_back(RayDestinationPoint{position, 3, 0});
	mWallPoints.emplace_back(RayDestinationPoint{sf::Vector2f(position.x + size.x, position.y), 0, 1});
	mWallPoints.emplace_back(RayDestinationPoint{sf::Vector2f(position.x + size.x, position.y + size.y), 1, 2});
	mWallPoints.emplace_back(RayDestinationPoint{sf::Vector2f(position.x, position.y + size.y), 2, 3});
}

void LightRenderer::submitLight(Light light)
{
	mLights.emplace_back(light);
}

void LightRenderer::flush(PointRenderer& pointRenderer)
{
	for(auto wallPoint : mWallPoints)
		pointRenderer.submitPoint(wallPoint.position, sf::Color::Red, 0.f, 5.f);

	mWallPoints.clear();
	mLights.clear();
}

}
