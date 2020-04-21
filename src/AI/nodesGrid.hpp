#pragma once

#include "obstacleGrid.hpp"

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <memory>
#include <set>
#include <functional>

namespace ph {

class NodesGrid
{
public:
	class Node
	{
		friend NodesGrid;

	public:
		const sf::Vector2i mPosition;
		const float mDistanceToDestination;
		const Node* mParent = nullptr;

	private:
		float mDistanceFromStart = 0.f;

	public:
		Node(const sf::Vector2i& position, const float distanceToDestination);
		float getFullCost() const;
		float getDistanceFromStart() const;
		bool isNewlyCreated() const;
	};

public:
	NodesGrid(const ObstacleGrid& obstacleGrid, sf::Vector2i destinationPosition);

	const Node& createStartNode(sf::Vector2i position);
	void changeDistanceFromStartInNode(Node& node, float newDistance);
	const Node& getNodeWithLowestCost() const;
	std::vector<std::reference_wrapper<Node>> getNodeNeighbours(const Node& node);
	void closeNode(const Node& node);

	bool hasAnyOpenedNode() const;

private:
	Node& createNode(sf::Vector2i position);

	using nodesCostsCompare = std::function<bool(const Node*, const Node*)>;
	static nodesCostsCompare createNodesCostsCompare();

	using nodesPosCompare = std::function<bool(const std::unique_ptr<Node>&, const std::unique_ptr<Node>&)>;
	static nodesPosCompare createNodesPosCompare();

private:
	const ObstacleGrid& mObstacleGrid;

	std::set<std::unique_ptr<Node>, nodesPosCompare> mNodes;
	std::set<Node*, nodesCostsCompare> mNodesByCost;

	const sf::Vector2i mDestinationPosition;
};
}
