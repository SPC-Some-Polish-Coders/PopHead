#pragma once 
 
#include "pathData.hpp"
#include <SFML/Graphics.hpp>

namespace ph { 

// This class is implementation of A* path finding algorithm

class AStar
{
public:
	AStar(const Grid& grid, const sf::Vector2u startNodePosition, const sf::Vector2u destinationNodePosition);

};

} 
