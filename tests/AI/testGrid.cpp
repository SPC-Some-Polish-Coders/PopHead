#include <catch.hpp>

#include "AI/grid.hpp"
#include <vector>

namespace ph {

TEST_CASE("Node operator < works correctly", "[AI][AStar]")
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

TEST_CASE("Grid can be created and node of certain position can be get")
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

}