#include "grid.hpp"
#include "Utilities/debug.hpp"

namespace ph {

Node::Node(const sf::Vector2u position)
	:mPosition(position)
	,mDistanceFromStart(0.f)
	,mDistanceToDestination(0.f)
{
}

bool operator < (const Node& lhs, const Node& rhs)
{
	if (lhs.getFullCost() < rhs.getFullCost())
		return true;
	if(lhs.getFullCost() == rhs.getFullCost())
		return lhs.mDistanceToDestination < rhs.mDistanceToDestination;
	return false;
}

bool operator == (const Node& lhs, const Node& rhs)
{
	return lhs.mIsObstacle == rhs.mIsObstacle && lhs.mPosition == rhs.mPosition;
}

bool operator != (const Node& lhs, const Node& rhs)
{
	return !(lhs == rhs);
}

Grid::Grid(const ObstacleGrid& obstacleGrid)
	: mObstacleGrid(obstacleGrid)
{
	for(unsigned x = 0; x < obstacleGrid.size(); ++x) {
		std::vector<Node> row;
		for(unsigned y = 0; y < obstacleGrid[x].size(); ++y)
			row.emplace_back(sf::Vector2u(x, y));
		mNodes.emplace_back(row);
	}
}

Node* Grid::getNodeOfPosition(const sf::Vector2u position)
{
	return &mNodes[position.x][position.y];
}

std::vector<Node*> Grid::getNeighboursOf(const Node& node)
{
	std::vector<Node*> neighbours;
	
	if(node.mPosition.x + 1 < mNodes.size())
		neighbours.push_back(&mNodes[node.mPosition.x + 1][node.mPosition.y]);
	if(node.mPosition.x > 0)
		neighbours.push_back(&mNodes[node.mPosition.x - 1][node.mPosition.y]);
	if(node.mPosition.y + 1 < mNodes[0].size())
		neighbours.push_back(&mNodes[node.mPosition.x][node.mPosition.y + 1]);
	if(node.mPosition.y > 0)
		neighbours.push_back(&mNodes[node.mPosition.x][node.mPosition.y - 1]);

	return neighbours;
}

bool Grid::isObstacle(const sf::Vector2u& position) const
{
	return mObstacleGrid[position.x][position.y];
}

}
