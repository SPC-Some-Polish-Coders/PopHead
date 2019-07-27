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
	AStar(const ObstacleGrid& grid);
	Path getPath(const sf::Vector2u startNodePosition, const sf::Vector2u destinationNodePosition);

private:
	Path retracePath(const Node& startNode, const Node& endNode);
	Path toDirectionPath(const std::deque<Node>& nodePath);
	Direction getDirectionBetweenNodes(const Node& startNode, const Node& endNode);
	bool isNodeInSet(const Node&, std::set<Node>);
	float getManhatanDistanceToDestination(const sf::Vector2u currentNodePosition);

private:
	const sf::Vector2u mStartNodePosition;
	const sf::Vector2u mDestinationNodePosition;
	Grid mGrid;
};

} 
