#include <catch.hpp>

#include "AI/grid.hpp"
#include <vector>

namespace ph {

TEST_CASE("Node operator < works correctly", "[AI][Grid]")
{
	Node node;
	node.mDistanceFromStart = 2;
	node.mDistanceToDestination = 1;

	Node nodeWithBiggerTotalCost;
	nodeWithBiggerTotalCost.mDistanceFromStart = 2;
	nodeWithBiggerTotalCost.mDistanceToDestination = 2;
	CHECK(node < nodeWithBiggerTotalCost);

	Node nodeWithTheSameTotalCostButWithBiggerEvaluatedDistanceToDestination;
	nodeWithTheSameTotalCostButWithBiggerEvaluatedDistanceToDestination.mDistanceFromStart = 1;
	nodeWithTheSameTotalCostButWithBiggerEvaluatedDistanceToDestination.mDistanceToDestination = 2;
	CHECK(node < nodeWithTheSameTotalCostButWithBiggerEvaluatedDistanceToDestination);

	Node nodeWithBiggerTotalCostAndShorterEvaluatedDistanceToDestination;
	nodeWithBiggerTotalCostAndShorterEvaluatedDistanceToDestination.mDistanceToDestination = 1;
	nodeWithBiggerTotalCostAndShorterEvaluatedDistanceToDestination.mDistanceFromStart = 4;
	CHECK(nodeWithBiggerTotalCost < nodeWithBiggerTotalCostAndShorterEvaluatedDistanceToDestination);
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
	CHECK(grid.getNodeOfPosition({0, 0})->mIsObstacle == true);
	CHECK(grid.getNodeOfPosition({0, 1})->mIsObstacle == false);
	CHECK(grid.getNodeOfPosition({1, 0})->mIsObstacle == false);
	CHECK(grid.getNodeOfPosition({1, 1})->mIsObstacle == true);
	CHECK(grid.getNodeOfPosition({2, 0})->mIsObstacle == false);
	CHECK(grid.getNodeOfPosition({2, 1})->mIsObstacle == false);
	CHECK(grid.getNodeOfPosition({3, 0})->mIsObstacle == true);
	CHECK(grid.getNodeOfPosition({3, 1})->mIsObstacle == true);
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
		auto right = grid.getNodeOfPosition({2, 1});
		auto left = grid.getNodeOfPosition({0, 1});
		auto up = grid.getNodeOfPosition({1, 0});
		auto down = grid.getNodeOfPosition({1, 2});
		sf::Vector2u currentNodePosition(1, 1);
		auto neighbours = grid.getNeighboursOf(*grid.getNodeOfPosition(currentNodePosition));
		CHECK_THAT(neighbours, Contains(std::vector<Node*>{right, left, up, down}));
	}
	{ // Current node is on left
		auto right = grid.getNodeOfPosition({1, 1});
		auto up = grid.getNodeOfPosition({0, 0});
		auto down = grid.getNodeOfPosition({2, 0});
		sf::Vector2u currentNodePosition(1, 0);
		auto neighbours = grid.getNeighboursOf(*grid.getNodeOfPosition(currentNodePosition));
		CHECK_THAT(neighbours, Contains(std::vector<Node*>{right, up, down}));
	}
	{ // Current node is on down right corner
		auto left = grid.getNodeOfPosition({1, 2});
		auto up = grid.getNodeOfPosition({2, 1});
		sf::Vector2u currentNodePosition(2, 2);
		auto neighbours = grid.getNeighboursOf(*grid.getNodeOfPosition(currentNodePosition));
		CHECK_THAT(neighbours, Contains(std::vector<Node*>{left, up}));
	}
}

}