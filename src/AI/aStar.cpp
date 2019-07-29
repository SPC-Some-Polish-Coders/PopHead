#include "aStar.hpp"
#include "Logs/logs.hpp"
#include <cmath>

namespace ph {

AStar::AStar(const ObstacleGrid& obstacleGrid)
	:mGrid(obstacleGrid)
{
}

Path AStar::getPath(const sf::Vector2u startNodePosition, const sf::Vector2u destinationNodePosition)
{
	mDestinationNodePosition = destinationNodePosition;

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
			if(neighbour->mIsObstacle || isNodeInSet(*neighbour, closedNodes))
				continue;

			if (!isNodeInSet(*neighbour, openNodesPointers)) {
				neighbour->mDistanceFromStart = currentNode->mDistanceFromStart + 1;
				neighbour->mDistanceToDestination = getManhatanDistanceToDestination(neighbour->mPosition);
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

	PH_LOG(LogLevel::Warning, "Path wasn't found!");
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

bool AStar::isNodeInSet(Node& node, const std::set<Node*>& set)
{
	auto found = set.find(&node);
	return found != set.cend();
}

float AStar::getManhatanDistanceToDestination(const sf::Vector2u currentNodePosition)
{
	// Change manhatan distance to some other heuristic when zombie can move in 8 directions
	float legX = std::abs(static_cast<float>(mDestinationNodePosition.x) - currentNodePosition.x);
	float legY = std::abs(static_cast<float>(mDestinationNodePosition.y) - currentNodePosition.y);
	return legX + legY;
}

}
