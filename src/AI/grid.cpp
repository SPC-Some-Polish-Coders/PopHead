#include "grid.hpp"
#include "Utilities/debug.hpp"

namespace ph {

Node::Node(const bool isObstacle, const sf::Vector2u position)
	:mIsObstacle(isObstacle)
	,mPosition(position)
	,mRealDistanceFromStartNode(0.f)
	,mEvaluatedDistanceToDestination(0.f)
{
}

Grid::Grid(const ObstacleGrid& obstacleGrid)
{
	for(unsigned x = 0; x < obstacleGrid.size(); ++x) {
		std::vector<Node> row;
		for(unsigned y = 0; y < obstacleGrid[x].size(); ++y)
			row.emplace_back(obstacleGrid[x][y], sf::Vector2u(x, y));
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
		neighbours.emplace_back(mNodes[node.mPosition.x + 1][node.mPosition.y].mIsObstacle, sf::Vector2u(node.mPosition.x + 1, node.mPosition.y));
	if(node.mPosition.x - 1 >= 0)
		neighbours.emplace_back(mNodes[node.mPosition.x - 1][node.mPosition.y].mIsObstacle, sf::Vector2u(node.mPosition.x - 1, node.mPosition.y));
	if(node.mPosition.y + 1 < mNodes.size())
		neighbours.emplace_back(mNodes[node.mPosition.x][node.mPosition.y + 1].mIsObstacle, sf::Vector2u(node.mPosition.x, node.mPosition.y + 1));
	if(node.mPosition.y - 1 >= 0)
		neighbours.emplace_back(mNodes[node.mPosition.x][node.mPosition.y - 1].mIsObstacle, sf::Vector2u(node.mPosition.x, node.mPosition.y - 1));

	return neighbours;
}

}