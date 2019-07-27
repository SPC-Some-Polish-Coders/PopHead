#include <catch.hpp>

#include "AI/grid.hpp"
#include <vector>

namespace ph {

TEST_CASE("Node operator < works correctly", "[AI][Grid]")
{
	Node node;
	node.mRealDistanceFromStartNode = 2;
	node.mEvaluatedDistanceToDestination = 1;

	Node nodeWithBiggerTotalCost;
	nodeWithBiggerTotalCost.mRealDistanceFromStartNode = 2;
	nodeWithBiggerTotalCost.mEvaluatedDistanceToDestination = 2;
	CHECK(node < nodeWithBiggerTotalCost);

	Node nodeWithTheSameTotalCostButWithBiggerEvaluatedDistanceToDestination;
	nodeWithTheSameTotalCostButWithBiggerEvaluatedDistanceToDestination.mRealDistanceFromStartNode = 1;
	nodeWithTheSameTotalCostButWithBiggerEvaluatedDistanceToDestination.mEvaluatedDistanceToDestination = 2;
	CHECK(node < nodeWithTheSameTotalCostButWithBiggerEvaluatedDistanceToDestination);
}

TEST_CASE("Grid can be created and node of certain position can be get", "[AI][Grid]")
{
	const ObstacleGrid obstacles{
		{true, false},
		{false, true},
		{false, false},
		{true, true}
	};

	Grid grid(obstacles);
	CHECK(grid.getNodeOfPosition({0, 0}).mIsObstacle == true);
	CHECK(grid.getNodeOfPosition({1, 0}).mIsObstacle == false);
	CHECK(grid.getNodeOfPosition({0, 1}).mIsObstacle == false);
	CHECK(grid.getNodeOfPosition({1, 1}).mIsObstacle == true);
	CHECK(grid.getNodeOfPosition({0, 2}).mIsObstacle == false);
	CHECK(grid.getNodeOfPosition({1, 2}).mIsObstacle == false);
	CHECK(grid.getNodeOfPosition({0, 3}).mIsObstacle == true);
	CHECK(grid.getNodeOfPosition({1, 3}).mIsObstacle == true);
}

TEST_CASE("Neighbours of node can be get", "[AI][Grid]")
{
	using namespace Catch::Matchers;

	// Remember that on this obstacle map up is left, down is right, left is up and right is down.
	const ObstacleGrid obstacles{
		{true, true, false},
		{false, true, false},
		{true, true, true},
	};

	Grid grid(obstacles);

	{ // Current node is on center
		Node right(true, {2, 1});
		Node left(true, {0, 1});
		Node up(false, {1, 0});
		Node down(false, {1, 2});
		sf::Vector2u currentNodePosition(1, 1);
		auto neighbours = grid.getNeighboursOf(grid.getNodeOfPosition(currentNodePosition));
		CHECK_THAT(neighbours, Contains(std::vector<Node>{right, left, up, down}));
	}
	{ // Current node is on left
		Node right(true, {1, 1});
		Node up(true, {0, 0});
		Node down(false, {0, 2});
		sf::Vector2u currentNodePosition(1, 0);
		auto neighbours = grid.getNeighboursOf(grid.getNodeOfPosition(currentNodePosition));
		CHECK_THAT(neighbours, Contains(std::vector<Node>{right, up, down}));
	}
	{ // Current node is on down right corner
		Node left(false, {1, 2});
		Node up(true, {2, 1});
		sf::Vector2u currentNodePosition(2, 2);
		auto neighbours = grid.getNeighboursOf(grid.getNodeOfPosition(currentNodePosition));
		CHECK_THAT(neighbours, Contains(std::vector<Node>{left, up}));
	}
}

}