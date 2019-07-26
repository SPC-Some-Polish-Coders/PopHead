#include "grid.hpp"
#include "Utilities/debug.hpp"

namespace ph {

Node::Node(const bool isObstacle)
	:mIsObstacle(isObstacle)
{
}

Grid::Grid(const ObstacleGrid& obstacleGrid)
{
	for(const auto& obstacleRow : obstacleGrid) {
		std::vector<Node> row;
		for(const auto& isObstacle : obstacleRow)
			row.emplace_back(Node(isObstacle));
		mNodes.emplace_back(row);
	}
}

Node Grid::getNodeOfPosition(const sf::Vector2u position)
{
	return mNodes[position.x][position.y];
}

std::vector<Node> Grid::getNeighboursOf(const Node& node)
{
	std::vector<Node> neighbours;

	if(node.mPosition.x + 1 < mNodes[0].size())
		neighbours.emplace_back(mNodes[node.mPosition.x + 1][node.mPosition.y]);
	if(node.mPosition.x - 1 >= 0)
	neighbours.emplace_back(mNodes[node.mPosition.x - 1][node.mPosition.y]);
	if(node.mPosition.y + 1 < mNodes.size())
	neighbours.emplace_back(mNodes[node.mPosition.x][node.mPosition.y + 1]);
	if(node.mPosition.y - 1 >= 0)
	neighbours.emplace_back(mNodes[node.mPosition.x][node.mPosition.y - 1]);

	return neighbours;
}

}