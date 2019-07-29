#include <catch.hpp>

#include <AI/aStar.hpp>

namespace ph {

ObstacleGrid getWalkableGrid3x4();
ObstacleGrid getWalkableGrid2x2();
ObstacleGrid getGrid5x3WithObstacleInCenter();

TEST_CASE("A* algorithm choose the quickest path when path is straight and there are no obstacles", "[AI][AStar]")
{
	auto grid = getWalkableGrid3x4();
	AStar a(grid);
	auto path = a.getPath({1, 0}, {1, 3});
	REQUIRE(path == Path{Direction::south, Direction::south, Direction::south});
}

TEST_CASE("A* algorithm choose the quickest path with one turn without obstacles", "[AI][AStar]")
{
	auto grid = getWalkableGrid3x4();
	AStar a(grid);
	auto path = a.getPath({0, 1}, {1, 0});
	REQUIRE((path == Path{Direction::north, Direction::east} || path == Path{Direction::east, Direction::north}));
}

TEST_CASE("A* algorithm choose the quickest path with 2 turns with obstacles", "[AI][AStar]")
{
	auto grid = getGrid5x3WithObstacleInCenter();
	AStar a(grid);
	auto path = a.getPath({3, 2}, {2, 0});
	REQUIRE(path == Path{Direction::east, Direction::north, Direction::north, Direction::west, Direction::west});
}

ObstacleGrid getWalkableGrid3x4()
{
	std::vector<bool> column{false, false, false, false};
	ObstacleGrid grid = {column, column, column};
	return grid;
}

ObstacleGrid getWalkableGrid2x2()
{
	std::vector<bool> column{false, false};
	ObstacleGrid grid = {column, column};
	return grid;
}

ObstacleGrid getGrid5x3WithObstacleInCenter()
{
	std::vector<bool> emptyColumn{false, false, false, false};
	std::vector<bool> columnWithObstacleInCenter{false, true, false, false};
	ObstacleGrid grid = {emptyColumn, columnWithObstacleInCenter, columnWithObstacleInCenter, columnWithObstacleInCenter, emptyColumn};
	return grid;
}

}