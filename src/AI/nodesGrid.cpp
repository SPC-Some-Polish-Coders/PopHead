#include "nodesGrid.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"

#include <algorithm>

namespace ph {
	
	NodesGrid::NodesGrid(const ObstacleGrid& obstacleGrid, sf::Vector2i destinationPosition)
		:mObstacleGrid(obstacleGrid)
		,mNodes(createNodesPosCompare())
		,mNodesByCost(createNodesCostsCompare())
		,mDestinationPosition(destinationPosition)
	{
	}

	const NodesGrid::Node& NodesGrid::createStartNode(sf::Vector2i position)
	{
		return createNode(position);
	}

	void NodesGrid::changeDistanceFromStartInNode(NodesGrid::Node& node, float newDistance)
	{
		mNodesByCost.erase(&node);
		node.mDistanceFromStart = newDistance;
		mNodesByCost.insert(&node);
	}

	const NodesGrid::Node& NodesGrid::getNodeWithLowestCost() const
	{
		return **mNodesByCost.begin();
	}

	std::vector<std::reference_wrapper<NodesGrid::Node>> NodesGrid::getNodeNeighbours(const NodesGrid::Node& node)
	{
		auto& position = node.mPosition;

		std::vector<sf::Vector2i> neighboursPositions = {
			{position.x, position.y - 1},
			{position.x + 1, position.y - 1},
			{position.x + 1, position.y},
			{position.x + 1, position.y + 1},
			{position.x, position.y + 1},
			{position.x - 1, position.y + 1},
			{position.x - 1, position.y},
			{position.x - 1, position.y - 1}
		};

		// is given position inside map and not an obstacle?
		bool north		= !mObstacleGrid.isObstacle(neighboursPositions[0].x, neighboursPositions[0].y);
		bool north_east = !mObstacleGrid.isObstacle(neighboursPositions[1].x, neighboursPositions[1].y);
		bool east		= !mObstacleGrid.isObstacle(neighboursPositions[2].x, neighboursPositions[2].y);
		bool south_east = !mObstacleGrid.isObstacle(neighboursPositions[3].x, neighboursPositions[3].y);
		bool south		= !mObstacleGrid.isObstacle(neighboursPositions[4].x, neighboursPositions[4].y);
		bool south_west = !mObstacleGrid.isObstacle(neighboursPositions[5].x, neighboursPositions[5].y);
		bool west		= !mObstacleGrid.isObstacle(neighboursPositions[6].x, neighboursPositions[6].y);
		bool north_west = !mObstacleGrid.isObstacle(neighboursPositions[7].x, neighboursPositions[7].y);

		std::vector<size_t> neighboursIndexes;
		if (north)
			neighboursIndexes.push_back(0);
		if (east)
			neighboursIndexes.push_back(2);
		if (south)
			neighboursIndexes.push_back(4);
		if (west)
			neighboursIndexes.push_back(6);
		if (north && east && north_east)
			neighboursIndexes.push_back(1);
		if (south && east && south_east)
			neighboursIndexes.push_back(3);
		if (south && west && south_west)
			neighboursIndexes.push_back(5);
		if (north && west && north_west)
			neighboursIndexes.push_back(7);

		std::vector<std::reference_wrapper<Node>> neighbours;

		for (const auto index : neighboursIndexes)
		{
			const auto& pos = neighboursPositions.at(index);
			neighbours.emplace_back(std::ref(createNode(pos)));
		}

		return neighbours;
	}

	void NodesGrid::closeNode(const NodesGrid::Node& node)
	{
		Node* pointer = const_cast<Node*>(&node);
		auto count = mNodesByCost.erase(pointer);

		PH_ASSERT_UNEXPECTED_SITUATION(count == 1, "Attempt to close non-existing node");
	}

	bool NodesGrid::hasAnyOpenedNode() const
	{
		return !mNodesByCost.empty();
	}

	NodesGrid::Node& NodesGrid::createNode(sf::Vector2i position)
	{
		auto pointer = std::make_unique<Node>(position, Math::distanceBetweenPoints(position, mDestinationPosition));
		auto& ref = *pointer.get();
		auto returnPair = mNodes.emplace(std::move(pointer));
		
		if (returnPair.second)
			mNodesByCost.insert(&ref);

		return **(returnPair.first);
	}

	NodesGrid::nodesCostsCompare NodesGrid::createNodesCostsCompare()
	{
		return nodesCostsCompare([](const Node* node1, const Node* node2) -> bool
			{
				if (node1->getFullCost() < node2->getFullCost())
					return true;
				if (node1->getFullCost() > node2->getFullCost())
					return false;
				return node1 < node2;
			}
		);
	}

	NodesGrid::nodesPosCompare NodesGrid::createNodesPosCompare()
	{
		return nodesPosCompare([](const std::unique_ptr<Node>& node1, const std::unique_ptr<Node>& node2) -> bool
			{
				auto& pos1 = node1->mPosition;
				auto& pos2 = node2->mPosition;
				return std::make_pair(pos1.x, pos1.y) < std::make_pair(pos2.x, pos2.y);
			}
		);
	}

	NodesGrid::Node::Node(const sf::Vector2i& position, const float distanceToDestination)
		: mPosition(position)
		, mDistanceToDestination(distanceToDestination)
	{
	}

	float NodesGrid::Node::getFullCost() const
	{
		return mDistanceFromStart + mDistanceToDestination;
	}

	float NodesGrid::Node::getDistanceFromStart() const
	{
		return mDistanceFromStart;
	}
	
	bool NodesGrid::Node::isNewlyCreated() const
	{
		return mDistanceFromStart == 0.f;
	}
}
