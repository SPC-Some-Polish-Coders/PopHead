#pragma once 
 
#include "pathData.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

// This is implementation of A* path finding algorithm

struct Node
{
	Node(const sf::Vector2f position, const sf::Vector2f parentPosition,
		const float realDistanceFromStartNode, const float straightLineDistanceToDestination);

	const sf::Vector2f mPosition;
	const sf::Vector2f mParentPosition;
	const float mRealDistanceFromStartNode;
	const float mStraightLineDistanceToDestination;
	const float mCost;
};

inline bool operator<(const Node& lhs, const Node& rhs)
{
	return false;
}

class AStar
{
public:
	AStar(const Grid& grid, const sf::Vector2u startNodePosition, const sf::Vector2u destinationNodePosition);
	Path getPath();

private:
	bool didWeReachTheDestination(const sf::Vector2u currentNodePosition);
	float getManhatanDistance(const sf::Vector2f currentNodePosition);

private:
	const sf::Vector2u mStartNodePosition;
	const sf::Vector2u mDestinationNodePosition;
	const Grid& mGrid;
};

} 
