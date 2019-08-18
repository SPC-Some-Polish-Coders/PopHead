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
	AStarAlgorithm(const ObstacleGrid& grid, const sf::Vector2u& startNodePosition, const sf::Vector2u& destinationNodePosition);
	Path getPath();

private:
	Path retracePath(const NodesGrid::Node& startNode, const NodesGrid::Node& endNode);
	Direction getDirectionBetweenNodes(const NodesGrid::Node& startNode, const NodesGrid::Node& endNode);

	float distanceBetweenNodes(const NodesGrid::Node& pos1, const NodesGrid::Node& pos2);

private:
	NodesGrid mNodesGrid;
	const sf::Vector2u mStartNodePosition;
	const sf::Vector2u mDestinationNodePosition;
};

} 
