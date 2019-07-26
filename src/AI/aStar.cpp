#include "aStar.hpp"
#include <cmath>

namespace ph {

Node::Node(const sf::Vector2f position, const sf::Vector2f parentPosition, 
           const float realDistanceFromStartNode, const float straightLineDistanceToDestination)
		:mPosition(position)
		,mParentPosition(parentPosition)
		,mRealDistanceFromStartNode(realDistanceFromStartNode)
		,mStraightLineDistanceToDestination(straightLineDistanceToDestination)
		,mCost(realDistanceFromStartNode + straightLineDistanceToDestination)
{
}

AStar::AStar(const Grid& grid, const sf::Vector2u startNodePosition, const sf::Vector2u destinationNodePosition)
	:mGrid(grid)
	,mStartNodePosition(startNodePosition)
	,mDestinationNodePosition(destinationNodePosition)
{

}

Path AStar::getPath()
{
	Path path;
	return path;
}

bool AStar::didWeReachTheDestination(const sf::Vector2u currentNodePosition)
{
	return currentNodePosition == mDestinationNodePosition;
}

float AStar::getManhatanDistance(const sf::Vector2f currentNodePosition)
{
	// Change manhatan distance to some other euristic when zombie can move in 8 directions
	float legX = std::abs(mDestinationNodePosition.x - currentNodePosition.x);
	float legY = std::abs(mDestinationNodePosition.y - currentNodePosition.y);
	return legX + legY;
}


}
