#pragma once 
 
#include "nodesGrid.hpp"
#include "obstacleGrid.hpp"
#include "pathData.hpp"

#include <SFML/Graphics.hpp>

#include <set>

namespace ph {

class AStarAlgorithm
{
public:
	AStarAlgorithm(const ObstacleGrids& grid);
	Path getPath(const sf::Vector2u startNodePosition, const sf::Vector2u destinationNodePosition);

private:
	Path retracePath(const Node* const startNode, Node* const endNode);
	Path toDirectionPath(const std::deque<Node*>& nodePath);
	Direction getDirectionBetweenNodes(const Node* const startNode, const Node* const endNode);
	bool isNodeInSet(Node&, const std::set<Node*>&);
	float getManhatanDistanceToDestination(const sf::Vector2u currentNodePosition, const sf::Vector2u destinationNodePosition);

private:
	NodesGrid mNodesGrid;
	const sf::Vector2u mStartNodePosition;
	const sf::Vector2u mDestinationNodePosition;
};

} 
