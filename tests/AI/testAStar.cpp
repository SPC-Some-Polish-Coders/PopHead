#include <catch.hpp>

#include "AI/aStarAlgorithm.hpp"
#include "AI/obstacleGrid.hpp"

namespace ph {
//
//ObstacleGrid getWalkableGrid3x4();
//ObstacleGrid getWalkableGrid2x2();
//ObstacleGrid getGrid5x3WithObstacleInCenter();
//ObstacleGrid getGrid6x6WithLongObstacle();
//ObstacleGrid getGrid8x8WithLabirinth();
//
//TEST_CASE("A* algorithm choose the quickest path when path is straight and there are no obstacles", "[AI][AStar]")
//{
//	auto grid = getWalkableGrid3x4();
//	AStarAlgorithm a(grid, { 1, 0 }, { 1, 3 });
//	auto path = a.getPath();
//	REQUIRE(path == Path{Direction::south, Direction::south, Direction::south});
//}
//
//ObstacleGrid getWalkableGrid3x4()
//{
//	ObstacleGrid grid(3, 4);
//	return grid;
//}
//
//TEST_CASE("A* algorithm choose the quickest path with one turn without obstacles", "[AI][AStar]")
//{
//	auto grid = getWalkableGrid3x4();
//	AStarAlgorithm a(grid, { 0, 1 }, { 1, 0 });
//	auto path = a.getPath();
//	REQUIRE((path == Path{Direction::north, Direction::east} || path == Path{Direction::east, Direction::north}));
//}
//
//ObstacleGrid getWalkableGrid2x2()
//{
//	ObstacleGrid grid(2, 2);
//	return grid;
//}
//
//TEST_CASE("A* algorithm choose the quickest path with 2 turns with obstacles", "[AI][AStar]")
//{
//	auto grid = getGrid5x3WithObstacleInCenter();
//	AStarAlgorithm a(grid, { 3, 2 }, { 2, 0 });
//	auto path = a.getPath();
//	REQUIRE(path == Path{Direction::east, Direction::north, Direction::north, Direction::west, Direction::west});
//}
//
//ObstacleGrid getGrid5x3WithObstacleInCenter()
//{
//	std::vector<bool> emptyColumn{false, false, false, false};
//	std::vector<bool> columnWithObstacleInCenter{false, true, false, false};
//	ObstacleGrid grid({emptyColumn, columnWithObstacleInCenter, columnWithObstacleInCenter, columnWithObstacleInCenter, emptyColumn}, 5, 4);
//	return grid;
//}
//
//TEST_CASE("A* algorithm choose the quickest path with many turns and long obstacle", "[AI][AStar]")
//{
//	auto grid = getGrid6x6WithLongObstacle();
//	AStarAlgorithm a(grid, { 3, 4 }, { 0, 1 });
//	auto path = a.getPath();
//	REQUIRE((path == Path{Direction::east, Direction::north, Direction::north, Direction::north, 
//		Direction::west, Direction::west, Direction::west, Direction::west} || 
//		path == Path{Direction::north, Direction::east, Direction::north, Direction::north,
//			Direction::west, Direction::west, Direction::west, Direction::west}
//	));
//}
//
//ObstacleGrid getGrid6x6WithLongObstacle()
//{
//	std::vector<bool> emptyColumn{false, false, false, false, false, false};
//	std::vector<bool> columnWithObstacle{false, false, true, false, false, false};
//	ObstacleGrid grid({columnWithObstacle, columnWithObstacle, columnWithObstacle, columnWithObstacle, emptyColumn, emptyColumn}, 6, 6);
//	return grid;
//}
//
//TEST_CASE("A* algorithm choose the quickest path through the labyrint", "[AI][AStar]")
//{
//	auto grid = getGrid8x8WithLabirinth();
//	AStarAlgorithm a(grid, { 4, 0 }, { 0, 4 });
//	auto path = a.getPath();
//	REQUIRE(
//		path == Path{Direction::east, Direction::east, Direction::south, Direction::south,
//			Direction::west, Direction::west, Direction::west, Direction::west,
//			Direction::south, Direction::south, Direction::south, Direction::west, Direction::west, Direction::north}
//	);
//}
//
//ObstacleGrid getGrid8x8WithLabirinth()
//{
//	std::vector<bool> column1{false, true, false, true, false, false, false, false};
//	std::vector<bool> column2{false, true, false, true, true, false, true, false};
//	std::vector<bool> column3{false, true, false, false, false, false, true, false};
//	std::vector<bool> column4{false, true, false, true, true, true, true, false};
//	std::vector<bool> column5{false, true, false, false, false, false, false, false};
//	std::vector<bool> column6{false, true, false, true, false, true, false, true};
//	std::vector<bool> column7{false, false, false, true, false, true, false, true};
//	std::vector<bool> column8{false, true, true, false, false, false, false, false};
//	ObstacleGrid grid({column1, column2, column3, column4, column5, column6, column7, column8}, 8, 8);
//	return grid;
//}

}