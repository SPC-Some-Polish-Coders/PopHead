#include <catch.hpp>

#include <AI/aStarAlgorithm.hpp>

namespace ph {

ObstacleGrids getWalkableGrid3x4();
ObstacleGrids getWalkableGrid2x2();
ObstacleGrids getGrid5x3WithObstacleInCenter();
ObstacleGrids getGrid6x6WithLongObstacle();
ObstacleGrids getGrid8x8WithLabirinth();

TEST_CASE("A* algorithm choose the quickest path when path is straight and there are no obstacles", "[AI][AStar]")
{
	auto grid = getWalkableGrid3x4();
	AStarAlgorithm a(grid);
	auto path = a.getPath({1, 0}, {1, 3});
	REQUIRE(path == Path{Direction::south, Direction::south, Direction::south});
}

ObstacleGrids getWalkableGrid3x4()
{
	std::vector<bool> column{false, false, false, false};
	ObstacleGrids grid = {column, column, column};
	return grid;
}

TEST_CASE("A* algorithm choose the quickest path with one turn without obstacles", "[AI][AStar]")
{
	auto grid = getWalkableGrid3x4();
	AStarAlgorithm a(grid);
	auto path = a.getPath({0, 1}, {1, 0});
	REQUIRE((path == Path{Direction::north, Direction::east} || path == Path{Direction::east, Direction::north}));
}

ObstacleGrids getWalkableGrid2x2()
{
	std::vector<bool> column{false, false};
	ObstacleGrids grid = {column, column};
	return grid;
}

TEST_CASE("A* algorithm choose the quickest path with 2 turns with obstacles", "[AI][AStar]")
{
	auto grid = getGrid5x3WithObstacleInCenter();
	AStarAlgorithm a(grid);
	auto path = a.getPath({3, 2}, {2, 0});
	REQUIRE(path == Path{Direction::east, Direction::north, Direction::north, Direction::west, Direction::west});
}

ObstacleGrids getGrid5x3WithObstacleInCenter()
{
	std::vector<bool> emptyColumn{false, false, false, false};
	std::vector<bool> columnWithObstacleInCenter{false, true, false, false};
	ObstacleGrids grid = {emptyColumn, columnWithObstacleInCenter, columnWithObstacleInCenter, columnWithObstacleInCenter, emptyColumn};
	return grid;
}

TEST_CASE("A* algorithm choose the quickest path with many turns and long obstacle", "[AI][AStar]")
{
	auto grid = getGrid6x6WithLongObstacle();
	AStarAlgorithm a(grid);
	auto path = a.getPath({3, 4}, {0, 1});
	REQUIRE((path == Path{Direction::east, Direction::north, Direction::north, Direction::north, 
		Direction::west, Direction::west, Direction::west, Direction::west} || 
		path == Path{Direction::north, Direction::east, Direction::north, Direction::north,
			Direction::west, Direction::west, Direction::west, Direction::west}
	));
}

ObstacleGrids getGrid6x6WithLongObstacle()
{
	std::vector<bool> emptyColumn{false, false, false, false, false, false};
	std::vector<bool> columnWithObstacle{false, false, true, false, false, false};
	ObstacleGrids grid = {columnWithObstacle, columnWithObstacle, columnWithObstacle, columnWithObstacle, emptyColumn, emptyColumn};
	return grid;
}

TEST_CASE("A* algorithm choose the quickest path through the labyrint", "[AI][AStar]")
{
	auto grid = getGrid8x8WithLabirinth();
	AStarAlgorithm a(grid);
	auto path = a.getPath({4, 0}, {0, 4});
	REQUIRE(
		path == Path{Direction::east, Direction::east, Direction::south, Direction::south,
			Direction::west, Direction::west, Direction::west, Direction::west,
			Direction::south, Direction::south, Direction::south, Direction::west, Direction::west, Direction::north}
	);
}

ObstacleGrids getGrid8x8WithLabirinth()
{
	std::vector<bool> column1{false, true, false, true, false, false, false, false};
	std::vector<bool> column2{false, true, false, true, true, false, true, false};
	std::vector<bool> column3{false, true, false, false, false, false, true, false};
	std::vector<bool> column4{false, true, false, true, true, true, true, false};
	std::vector<bool> column5{false, true, false, false, false, false, false, false};
	std::vector<bool> column6{false, true, false, true, false, true, false, true};
	std::vector<bool> column7{false, false, false, true, false, true, false, true};
	std::vector<bool> column8{false, true, true, false, false, false, false, false};
	ObstacleGrids grid = {column1, column2, column3, column4, column5, column6, column7, column8};
	return grid;
}

}