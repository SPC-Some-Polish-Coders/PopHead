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
	Path retracePath(const Node* const startNode, Node* const endNode);
	Path toDirectionPath(const std::deque<Node*>& nodePath);
	Direction getDirectionBetweenNodes(const Node* const startNode, const Node* const endNode);
	bool isNodeInSet(const Node&, const std::set<Node*>&);
	float getManhatanDistanceToDestination(const sf::Vector2u currentNodePosition);

private:
	const sf::Vector2u mStartNodePosition;
	const sf::Vector2u mDestinationNodePosition;
	Grid mGrid;
};

} 
