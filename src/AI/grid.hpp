#pragma once

#include "pathData.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace ph {

struct Node
{
	Node(const sf::Vector2u position, const sf::Vector2u parentPosition,
		const float realDistanceFromStartNode, const float evaluatedDistanceToDestination);

	sf::Vector2u mPosition;
	sf::Vector2u mParentPosition;
	float mRealDistanceFromStartNode;
	float mEvaluatedDistanceToDestination;
	bool mIsObstacle;

	float getFullCost() const { return mRealDistanceFromStartNode + mEvaluatedDistanceToDestination; }
};

inline bool operator<(const Node& lhs, const Node& rhs)
{
	// TODO: refactor this function
	if(lhs.getFullCost() == rhs.getFullCost())
		return lhs.mEvaluatedDistanceToDestination < rhs.mEvaluatedDistanceToDestination;
	else
		return lhs.getFullCost() < rhs.getFullCost();
}

class Grid
{
public:
	Grid(const ObstacleGrid&);

private:
	std::vector<std::vector<Node>> mNodes;
};

}