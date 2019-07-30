#pragma once

#include "pathData.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace ph {

struct Node
{
	Node() = default;
	Node(const bool isObstacle, const sf::Vector2u position);

	sf::Vector2u mPosition;
	sf::Vector2u mParentPosition;
	float mDistanceFromStart;
	float mDistanceToDestination;
	bool mIsObstacle;

	float getFullCost() const { return mDistanceFromStart + mDistanceToDestination; }
};

bool operator < (const Node& lhs, const Node& rhs);
bool operator == (const Node& lhs, const Node& rhs);
bool operator != (const Node& lhs, const Node& rhs);

class Grid
{
public:
	Grid(const ObstacleGrid&);
	Node* getNodeOfPosition(const sf::Vector2u position);
	std::vector<Node*> getNeighboursOf(const Node&);

private:
	std::vector<std::vector<Node>> mNodes;
};

}