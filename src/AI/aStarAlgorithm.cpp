#include "pch.hpp"
#include "AStarAlgorithm.hpp"

namespace ph {

AStarAlgorithm::AStarAlgorithm(const ObstacleGrid& obstacleGrid, const Vec2i& startNodePosition, const Vec2i& destinationNodePosition)
	: mNodesGrid(obstacleGrid, destinationNodePosition)
	, mStartNodePosition(startNodePosition)
	, mDestinationNodePosition(destinationNodePosition)
{
}

Path AStarAlgorithm::getPath()
{
	auto& startNode = mNodesGrid.createStartNode(mStartNodePosition);

	while (mNodesGrid.hasAnyOpenedNode())
	{
		auto& currentNode = mNodesGrid.getNodeWithLowestCost();
		mNodesGrid.closeNode(currentNode);

		if (currentNode.mPosition == mDestinationNodePosition)
			return retracePath(startNode, currentNode);

		auto neighbours = mNodesGrid.getNodeNeighbours(currentNode);

		for (auto& neighbourIter : neighbours)
		{
			auto& neighbour = neighbourIter.get();
			auto newPath = currentNode.getDistanceFromStart() + distanceBetweenNodes(currentNode, neighbour);
			
			if (neighbour.isNewlyCreated() || newPath < neighbour.getDistanceFromStart())
			{
				mNodesGrid.changeDistanceFromStartInNode(neighbour, newPath);
				neighbour.mParent = &currentNode;
				continue;
			}
		}
	}

	return Path();
}

Path AStarAlgorithm::retracePath(const NodesGrid::Node& startNode, const NodesGrid::Node& endNode)
{
	Path path;
	
	const NodesGrid::Node* current = &endNode;
	while (current != &startNode)
	{
		path.emplace_front(getDirectionBetweenNodes(*current->mParent, *current));
		current = current->mParent;
	}

	return path;
}

Direction AStarAlgorithm::getDirectionBetweenNodes(const NodesGrid::Node& startNode, const NodesGrid::Node& endNode)
{
	auto& startPos = startNode.mPosition;
	auto& endPos = endNode.mPosition;

	i16 verticalDirection = 0;    // 0 - none, 1 - north, 2 - south
	i16 horizontalDirection = 0;  // 0 - none, 1 - east, 2 - west

	if (endPos.x > startPos.x)
		horizontalDirection = 1;
	else if (endPos.x < startPos.x)
		horizontalDirection = 2;

	if (endPos.y > startPos.y)
		verticalDirection = 2;
	else if (endPos.y < startPos.y)
		verticalDirection = 1;

	if (verticalDirection == 1 && horizontalDirection == 0)
		return Direction::north;
	if (verticalDirection == 1 && horizontalDirection == 1)
		return Direction::north_east;
	if (verticalDirection == 0 && horizontalDirection == 1)
		return Direction::east;
	if (verticalDirection == 2 && horizontalDirection == 1)
		return Direction::south_east;
	if (verticalDirection == 2 && horizontalDirection == 0)
		return Direction::south;
	if (verticalDirection == 2 && horizontalDirection == 2)
		return Direction::south_west;
	if (verticalDirection == 0 && horizontalDirection == 2)
		return Direction::west;
	if (verticalDirection == 1 && horizontalDirection == 2)
		return Direction::north_west;

	PH_UNEXPECTED_SITUATION("Two identical nodes were given");
}

float AStarAlgorithm::distanceBetweenNodes(const NodesGrid::Node& pos1, const NodesGrid::Node& pos2)
{
	return distanceBetweenPoints(pos1.mPosition, pos2.mPosition);
}

}
