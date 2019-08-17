#include "AStarAlgorithm.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"

#include <cmath>

namespace ph {

AStarAlgorithm::AStarAlgorithm(const ObstacleGrid& obstacleGrid, const sf::Vector2u& startNodePosition, const sf::Vector2u& destinationNodePosition)
	: mNodesGrid(obstacleGrid, destinationNodePosition)
	, mStartNodePosition(startNodePosition)
	, mDestinationNodePosition(destinationNodePosition)
{
}

Path AStarAlgorithm::getPath(const sf::Vector2u startNodePosition, const sf::Vector2u destinationNodePosition)
{
	auto compareCosts = [](const Node* lhs, const Node* rhs) {return *lhs < *rhs; };
	std::multiset<Node*, decltype(compareCosts)> openNodes(compareCosts);
	std::set<Node*> openNodesPointers;
	std::set<Node*> closedNodes;

	Node* startNode = mGrid.getNodeOfPosition(startNodePosition);
	openNodesPointers.emplace(startNode);
	openNodes.emplace(startNode);

	while(!openNodesPointers.empty()) {
		Node* currentNode = *openNodes.begin();
		openNodes.erase(openNodes.begin());
		openNodesPointers.erase(currentNode);
		closedNodes.emplace(currentNode);

		if(currentNode->mPosition == destinationNodePosition)
			return retracePath(mGrid.getNodeOfPosition(startNodePosition), mGrid.getNodeOfPosition(destinationNodePosition));

		for(Node* neighbour : mGrid.getNeighboursOf(*currentNode)) {
			if(mGrid.isObstacle(neighbour->mPosition) || isNodeInSet(*neighbour, closedNodes))
				continue;

			if (!isNodeInSet(*neighbour, openNodesPointers)) {
				neighbour->mDistanceFromStart = currentNode->mDistanceFromStart + 1;
				neighbour->mDistanceToDestination = getManhatanDistanceToDestination(neighbour->mPosition, destinationNodePosition);
				neighbour->mParentPosition = currentNode->mPosition;
				openNodes.emplace(neighbour);
				openNodesPointers.emplace(neighbour);

			}
			if(neighbour->mDistanceFromStart > currentNode->mDistanceFromStart + 1) {
				auto iter = openNodes.begin();
				for (; iter != openNodes.end(); ++iter)
				{
					if(*iter == neighbour) {
						break;
					}
				}
				openNodes.erase(iter);
				neighbour->mDistanceFromStart = currentNode->mDistanceFromStart + 1;
				neighbour->mParentPosition = currentNode->mPosition;
				openNodes.emplace(neighbour);
			}
		}
	}

	PH_LOG_WARNING("Path wasn't found!");
	return Path();
}

Path AStarAlgorithm::retracePath(const Node* const startNode, Node* const endNode)
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

Path AStarAlgorithm::toDirectionPath(const std::deque<Node*>& nodePath)
{
	Path path;
	for(unsigned i = 0; i < nodePath.size() - 1; ++i) {
		Direction direction = getDirectionBetweenNodes(nodePath[i], nodePath[i + 1]);
		path.emplace_back(direction);
	}
	return path;
}

Direction AStarAlgorithm::getDirectionBetweenNodes(const Node* const startNode, const Node* const endNode)
{
	if(endNode->mPosition.x > startNode->mPosition.x)
		return Direction::east;
	else if(endNode->mPosition.x < startNode->mPosition.x)
		return Direction::west;
	else if(endNode->mPosition.y > startNode->mPosition.y)
		return Direction::south;
	else if(endNode->mPosition.y < startNode->mPosition.y)
		return Direction::north;
	PH_UNEXPECTED_SITUATION("Two identical nodes were given");
}

bool AStarAlgorithm::isNodeInSet(Node& node, const std::set<Node*>& set)
{
	auto found = set.find(&node);
	return found != set.cend();
}

float AStarAlgorithm::getManhatanDistanceToDestination(const sf::Vector2u currentNodePosition, const sf::Vector2u destinationNodePosition)
{
	// Change manhatan distance to some other heuristic when zombie can move in 8 directions
	float legX = std::abs(static_cast<float>(destinationNodePosition.x) - currentNodePosition.x);
	float legY = std::abs(static_cast<float>(destinationNodePosition.y) - currentNodePosition.y);
	return legX + legY;
}

}
