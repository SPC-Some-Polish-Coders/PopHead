#pragma once 
 
#include "pathData.hpp"
#include <SFML/Graphics.hpp>

namespace ph { 

class RandomPathAlgorithm
{
public:
	RandomPathAlgorithm(const ObstacleGrid&);

	Path getRandomPath(const sf::Vector2u startNodePosition);

private:
	const ObstacleGrid& mObstacleGrid;
};

} 
