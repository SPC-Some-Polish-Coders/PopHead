#include "aStar.hpp"
#include <cmath>

namespace ph {

AStar::AStar(const ObstacleGrid& obstacleGrid, const sf::Vector2u startNodePosition, const sf::Vector2u destinationNodePosition)
	:mGrid(obstacleGrid)
	,mStartNodePosition(startNodePosition)
	,mDestinationNodePosition(destinationNodePosition)
{
}

Path AStar::getPath()
{
	std::set<Node> openNodes;
	std::set<Node> closedNodes;

	Node startNode(mStartNodePosition, {0, 0}, 0, getManhatanDistanceToDestination(mStartNodePosition));
	openNodes.emplace(startNode);

	while(!openNodes.empty()) {
		Node currentNode = *openNodes.begin();
		openNodes.erase(openNodes.begin());
		closedNodes.emplace(currentNode);
		if(didWeReachTheDestination(currentNode.mPosition))
			return Path();
	}
}

bool AStar::didWeReachTheDestination(const sf::Vector2u currentNodePosition)
{
	return currentNodePosition == mDestinationNodePosition;
}

float AStar::getManhatanDistanceToDestination(const sf::Vector2u currentNodePosition)
{
	// Change manhatan distance to some other heuristic when zombie can move in 8 directions
	float legX = std::abs(static_cast<int>(mDestinationNodePosition.x - currentNodePosition.x));
	float legY = std::abs(static_cast<int>(mDestinationNodePosition.y - currentNodePosition.y));
	return legX + legY;
}


}
