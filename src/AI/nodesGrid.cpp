#include "nodesGrid.hpp"
#include "Utilities/math.hpp"

#include <algorithm>

namespace ph {
	
	NodesGrid::NodesGrid(const ObstacleGrid& obstacleGrid, const sf::Vector2u& destinationPosition)
		: mObstacleGrid(obstacleGrid)
		, mNodes(createNodesPosCompare())
		, mNodesByCost(createNodesCostsCompare())
		, mGeneratedNodes(obstacleGrid.getColumnsCount() * obstacleGrid.getRowsCount(), false)
		, mClosedNodes(obstacleGrid.getColumnsCount() * obstacleGrid.getRowsCount(), false)
		, mDestinationPosition(destinationPosition)
	{
	}

	const NodesGrid::Node& NodesGrid::createStartNode(const sf::Vector2u& position)
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
		return **(mNodesByCost.begin());
	}

	std::vector<NodesGrid::Node&> NodesGrid::getNodeNeighbours(const NodesGrid::Node& node)
	{
		auto& position = node.mPosition;

		std::vector<sf::Vector2u> neighboursPositions = {
			{position.x, position.y - 1},
			{position.x + 1, position.y - 1},
			{position.x + 1, position.y},
			{position.x + 1, position.y + 1},
			{position.x, position.y + 1},
			{position.x - 1, position.y + 1},
			{position.x - 1, position.y},
			{position.x - 1, position.y - 1}
		};

		std::vector<Node&> neighbours;
		for (const auto& pos : neighboursPositions)
		{
			if (!isInBoundaries(pos))
				continue;
			if (mGeneratedNodes.at(internalIndex(pos)))
			{
				// optimize this
				auto iter = std::find_if(mNodes.begin(), mNodes.end(), [](const std::unique_ptr<Node>& node1, const std::unique_ptr<Node>& node2) { return node1->mPosition == node2->mPosition; });
				auto& ref = *iter->get();
				neighbours.emplace_back(ref);
			}
			else
				neighbours.emplace_back(createNode(pos));
		}

		return neighbours;
	}

	void NodesGrid::closeNode(const NodesGrid::Node& node)
	{
		mClosedNodes.at(internalIndex(node.mPosition)) = true;
	}

	size_t NodesGrid::internalIndex(const sf::Vector2u& position) const
	{
		return static_cast<size_t>(position.x + position.y * mObstacleGrid.getColumnsCount());
	}

	NodesGrid::Node& NodesGrid::createNode(const sf::Vector2u& position)
	{
		auto pointer = std::make_unique<Node>(position, Math::distanceBetweenPoints(position, mDestinationPosition));  // calculate distance
		auto& ref = *pointer.get();
		mNodes.emplace(std::move(pointer));
		mNodesByCost.insert(&ref);
		return ref;
	}

	bool NodesGrid::isInBoundaries(const sf::Vector2u& position) const
	{
		return position.x < mObstacleGrid.getColumnsCount() 
			&& position.y < mObstacleGrid.getRowsCount();
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
