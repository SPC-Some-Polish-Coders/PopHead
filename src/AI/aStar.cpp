#include "aStar.hpp"
#include "Utilities/debug.hpp"
#include <cmath>

namespace ph {

AStar::AStar(const ObstacleGrid& obstacleGrid)
	:mGrid(obstacleGrid)
{
}

Path AStar::getPath(const sf::Vector2u startNodePosition, const sf::Vector2u destinationNodePosition)
{
	std::set<Node*> openNodes;
	std::set<Node*> closedNodes;

	Node* startNode = mGrid.getNodeOfPosition(startNodePosition);
	openNodes.emplace(startNode);

	while(!openNodes.empty()) {
		Node* currentNode = *openNodes.begin();
		openNodes.erase(openNodes.begin());
		closedNodes.emplace(currentNode);

		if(currentNode->mPosition == destinationNodePosition)
			return retracePath(mGrid.getNodeOfPosition(startNodePosition), mGrid.getNodeOfPosition(destinationNodePosition));

		for(Node* neighbour : mGrid.getNeighboursOf(*currentNode)) {
			if(neighbour->mIsObstacle || isNodeInSet(*neighbour, closedNodes))
				continue;

			if(neighbour->mRealDistanceFromStartNode > currentNode->mRealDistanceFromStartNode + 1 || !isNodeInSet(*neighbour, openNodes)) {
				neighbour->mRealDistanceFromStartNode = currentNode->mRealDistanceFromStartNode + 1;
				neighbour->mEvaluatedDistanceToDestination = getManhatanDistanceToDestination(neighbour->mPosition);
				neighbour->mParentPosition = currentNode->mPosition;
				if(!isNodeInSet(*neighbour, openNodes))
					openNodes.emplace(neighbour);
			}
		}
	}

	PH_LOG(LogType::Warning, "Path wasn't found!");
	return Path();
}

Path AStar::retracePath(const Node* const startNode, Node* const endNode)
{
	std::deque<Node*> nodePath;
	Node* currentNode = endNode;
	while(currentNode != startNode) {
		nodePath.emplace_front(currentNode);
		currentNode = mGrid.getNodeOfPosition(currentNode->mParentPosition);
	}
	nodePath.emplace_front(currentNode);
	return toDirectionPath(nodePath);
}

Path AStar::toDirectionPath(const std::deque<Node*>& nodePath)
{
	Path path;
	for(int i = 0; i < nodePath.size() - 1; ++i) {
		Direction direction = getDirectionBetweenNodes(nodePath[i], nodePath[i + 1]);
		path.emplace_back(direction);
	}
	return path;
}

Direction AStar::getDirectionBetweenNodes(const Node* const startNode, const Node* const endNode)
{
	if(endNode->mPosition.x > startNode->mPosition.x)
		return Direction::east;
	else if(endNode->mPosition.x < startNode->mPosition.x)
		return Direction::west;
	else if(endNode->mPosition.y > startNode->mPosition.y)
		return Direction::south;
	else if(endNode->mPosition.y < startNode->mPosition.y)
		return Direction::north;
}

bool AStar::isNodeInSet(const Node& node, const std::set<Node*>& set)
{
	for(const auto& n : set)
		if(*n == node)
			return true;
	return false;
}

float AStar::getManhatanDistanceToDestination(const sf::Vector2u currentNodePosition)
{
	// Change manhatan distance to some other heuristic when zombie can move in 8 directions
	float legX = std::abs(static_cast<float>(mDestinationNodePosition.x - currentNodePosition.x));
	float legY = std::abs(static_cast<float>(mDestinationNodePosition.y - currentNodePosition.y));
	return legX + legY;
}

}
