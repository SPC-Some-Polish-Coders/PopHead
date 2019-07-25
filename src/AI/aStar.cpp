#include "aStar.hpp"

namespace ph {

Node::Node(const sf::Vector2f position, const sf::Vector2f parentPosition, 
           const float realDistanceFromStartNode, const float straightLineDistanceToDestination)
		:mPosition(position)
		,mParentPosition(parentPosition)
		,mRealDistanceFromStartNode(realDistanceFromStartNode)
		,mStraightLineDistanceToDestination(straightLineDistanceToDestination)
		,mCost(realDistanceFromStartNode + straightLineDistanceToDestination) {}

AStar::AStar(const Grid& grid, const sf::Vector2u startNodePosition, const sf::Vector2u destinationNodePosition)
	:mGrid(grid)
	,mStartNodePosition(startNodePosition)
	,mDestinationNodePosition(destinationNodePosition)
{
}

Path AStar::getPath()
{
	return Path();
}

}
