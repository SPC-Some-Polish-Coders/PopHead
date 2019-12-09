#include "lightRenderer.hpp"
#include "Renderer/renderer.hpp"
#include "Utilities/math.hpp"
#include <optional>
#include <cmath>

namespace ph { 

void LightRenderer::submitWallQuad(sf::Vector2f position, sf::Vector2f size)
{
	sf::Vector2f upLeftPoint = position;
	sf::Vector2f upRightPoint = sf::Vector2f(position.x + size.x, position.y);
	sf::Vector2f downRightPoint = position + size;
	sf::Vector2f downLeftPoint = sf::Vector2f(position.x, position.y + size.y);

	size_t firstWallID = mWallPoints.size();

	mWalls.emplace_back(Wall{upLeftPoint, upRightPoint});
	mWalls.emplace_back(Wall{upRightPoint, downRightPoint});
	mWalls.emplace_back(Wall{downRightPoint, downLeftPoint});
	mWalls.emplace_back(Wall{downLeftPoint, upLeftPoint});

	mWallPoints.emplace_back(RayDestinationPoint{upLeftPoint, firstWallID + 3, firstWallID});
	mWallPoints.emplace_back(RayDestinationPoint{upRightPoint, firstWallID, firstWallID + 1});
	mWallPoints.emplace_back(RayDestinationPoint{downRightPoint, firstWallID + 1, firstWallID + 2});
	mWallPoints.emplace_back(RayDestinationPoint{downLeftPoint, firstWallID + 2, firstWallID + 3});
}

void LightRenderer::submitLight(Light light)
{
	mLights.emplace_back(light);
}

void LightRenderer::flush()
{
#if 0 
	// DEBUG STUFF
	for(auto wallPoint : mWallPoints)
		Renderer::submitPoint(wallPoint.position, sf::Color::Red, 0.f, 5.f);

	for(const auto& light : mLights)
		Renderer::submitPoint(light.position, light.color, 0.f, 15.f);

	for(Wall& wall : mWalls)
		Renderer::submitLine(sf::Color::Red, wall.leftPointPosition, wall.rightPointPosition, 2);
#endif

	for(Light& light : mLights) 
	{
		for(RayDestinationPoint wallPoint : mWallPoints) 
		{
			const sf::Vector2f rayDirection = Math::getUnitVector(wallPoint.position - light.position);
			const Ray ray = {light.position, rayDirection};
			float distanceToTheClosestPointOfIntersection = 10000.f;
			sf::Vector2f theClosestPointOfIntersection;
			for(Wall& wall : mWalls)
			{
				auto pointOfIntersection = getPointOfIntersection(ray, wall);
				if(pointOfIntersection) {
					const float distanceToPointOfIntersection = Math::distanceBetweenPoints(ray.position, *pointOfIntersection);
					if(distanceToPointOfIntersection < distanceToTheClosestPointOfIntersection) {
						distanceToTheClosestPointOfIntersection = distanceToPointOfIntersection;
						theClosestPointOfIntersection = *pointOfIntersection;
					}
				}
				else
					continue;
			}
			//Renderer::submitLine(light.color, light.position, theClosestPointOfIntersection, 1.f);
		}
	}

	mWallPoints.clear();
	mWalls.clear();
	mLights.clear();
}

auto LightRenderer::getPointOfIntersection(const Ray& ray, const Wall& wall) -> std::optional<sf::Vector2f>
{
	const float x1 = wall.leftPointPosition.x;
	const float y1 = wall.leftPointPosition.y;
	const float x2 = wall.rightPointPosition.x;
	const float y2 = wall.rightPointPosition.y;

	const float x3 = ray.position.x;
	const float y3 = ray.position.y;
	const float x4 = ray.position.x + ray.direction.x;
	const float y4 = ray.position.y + ray.direction.y;

	const float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if(den == 0.f)
		return std::nullopt;

	const float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
	const float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
	
	if(t > 0 && t < 1 && u > 0)
		return sf::Vector2f(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
	else
		return std::nullopt;
}

}
