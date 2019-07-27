#include <catch.hpp>

#include <AI/aStar.hpp>

namespace ph {

ObstacleGrid getWalkableGrid3x4();

TEST_CASE("A* algorithm choose the quickest path when path is straight and there are no obstacles", "[AI][AStar]")
{
	auto grid = getWalkableGrid3x4();
	AStar a(grid);
	auto path = a.getPath({1, 0}, {1, 3});
	REQUIRE(path == Path{Direction::south, Direction::south});
}

ObstacleGrid getWalkableGrid3x4()
{
	std::vector<bool> column{false, false, false, false};
	ObstacleGrid grid = {column, column, column};
	return grid;
}

}