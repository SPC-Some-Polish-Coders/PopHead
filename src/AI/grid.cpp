#include "grid.hpp"

namespace ph {

Node::Node(const sf::Vector2u position, const sf::Vector2u parentPosition,
	const float realDistanceFromStartNode, const float evaluatedDistanceToDestination)
	:mPosition(position)
	, mParentPosition(parentPosition)
	, mRealDistanceFromStartNode(realDistanceFromStartNode)
	, mEvaluatedDistanceToDestination(evaluatedDistanceToDestination)
{
}

Grid::Grid(const ObstacleGrid&)
{
}

}