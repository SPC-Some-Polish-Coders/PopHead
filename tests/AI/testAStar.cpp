#include <catch.hpp>

#include <AI/aStar.hpp>

namespace ph {

Grid getWalkableGrid3x4();

TEST_CASE("Node operator < works correctly", "[AI][AStar]")
{
	Node node({1, 0}, {0, 0}, 2, 1);
	
	Node nodeWithBiggerTotalCost({1, 0}, {0, 0}, 2, 2);
	CHECK(node < nodeWithBiggerTotalCost);

	Node nodeWithTheSameTotalCostButWithBiggerEvaluatedDistanceToDestination({1, 0}, {0, 0}, 1, 2);
	CHECK(node < nodeWithTheSameTotalCostButWithBiggerEvaluatedDistanceToDestination);
}

TEST_CASE("A* algorithm choose the quickest path when path is straight and there are no obstacles", "[AI][AStar]")
{
	auto grid = getWalkableGrid3x4();
	AStar a(grid, {0, 1}, {3, 1});
	auto path = a.getPath();
	REQUIRE(path == Path{Direction::south, Direction::south, Direction::south});
}

Grid getWalkableGrid3x4()
{
	std::vector<AreaType> row{AreaType::walkable, AreaType::walkable, AreaType::walkable};
	Grid grid = {row, row, row, row};
	return grid;
}

}