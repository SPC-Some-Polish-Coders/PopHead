#pragma once 
 
#include "grid.hpp"
#include "pathData.hpp"
#include <SFML/Graphics.hpp>
#include <set>

namespace ph {

// This is implementation of A* path finding algorithm

class AStar
{
public:
	AStar(const ObstacleGrid& grid, const sf::Vector2u startNodePosition, const sf::Vector2u destinationNodePosition);
	Path getPath();

private:
	bool didWeReachTheDestination(const sf::Vector2u currentNodePosition);
	float getManhatanDistanceToDestination(const sf::Vector2u currentNodePosition);

private:
	const sf::Vector2u mStartNodePosition;
	const sf::Vector2u mDestinationNodePosition;
	Grid mGrid;
};

} 
