#include <catch.hpp>

#include "AI/RandomPathAlgorithm.hpp"
#include "AI/obstacleGrid.hpp"

#include <algorithm>

namespace ph {

ObstacleGrid getWalkableGrid11x11();
ObstacleGrid getGridWithObstacles11x11();

TEST_CASE("Random path finding algorithm works properly with no obstacles", "[AI][RandomPathAlgorithm]")
{
	auto obstacleGrid = getWalkableGrid11x11();
	RandomPathAlgorithm rpa(obstacleGrid, {6, 6});
	Path path = rpa.getRandomPath();
	
	REQUIRE((
		( path == Path{Direction::east, Direction::east} ) ||
		( path == Path{Direction::east, Direction::east, Direction::east} ) ||
		( path == Path{Direction::east, Direction::east, Direction::east, Direction::east} ) ||
		( path == Path{Direction::east, Direction::east, Direction::east, Direction::east, Direction::east} ) ||
		( path == Path{Direction::west, Direction::west} ) ||
		( path == Path{Direction::west, Direction::west, Direction::west}) ||
		( path == Path{Direction::west, Direction::west, Direction::west, Direction::west}) ||
		( path == Path{Direction::west, Direction::west, Direction::west, Direction::west, Direction::west}) ||
		( path == Path{Direction::north, Direction::north} ) ||
		( path == Path{Direction::north, Direction::north, Direction::north} ) ||
		( path == Path{Direction::north, Direction::north, Direction::north, Direction::north} ) ||
		( path == Path{Direction::north, Direction::north, Direction::north, Direction::north, Direction::north} ) ||
		( path == Path{Direction::south, Direction::south} ) ||
		( path == Path{Direction::south, Direction::south, Direction::south} ) ||
		( path == Path{Direction::south, Direction::south, Direction::south, Direction::south} ) ||
		( path == Path{Direction::south, Direction::south, Direction::south, Direction::south, Direction::south} ) ||
		( path == Path{Direction::none, Direction::none, Direction::none} ) ||
		( path == Path{Direction::none, Direction::none} ) ||
		( path == Path{Direction::none} )
	));
}

ObstacleGrid getWalkableGrid11x11()
{
	ObstacleGrid grid(11, 11);
	return grid;
}

//TEST_CASE("Random path finding algorithm works properly with obstacles", "[AI][RandomPathAlgorithm]")
//{
//	for(int i = 0; i < 10; ++i) {
//		auto obstacleGrid = getGridWithObstacles11x11();
//		RandomPathAlgorithm rpa(obstacleGrid, {5, 5});
//		Path path = rpa.getRandomPath();
//		
//		CHECK((
//			(path == Path{Direction::east, Direction::east}) ||
//			(path == Path{Direction::east, Direction::east, Direction::east}) ||
//			(path == Path{Direction::east, Direction::east, Direction::east, Direction::east}) ||
//			(path == Path{Direction::west, Direction::west}) ||
//			(path == Path{Direction::west, Direction::west, Direction::west}) ||
//			(path == Path{Direction::north, Direction::north}) ||
//			(path == Path{Direction::none, Direction::none, Direction::none} ) ||
//			(path == Path{Direction::none, Direction::none} ) ||
//			(path == Path{Direction::none} )
//		));
//	}
//}

ObstacleGrid getGridWithObstacles11x11()
{
	auto grid = getWalkableGrid11x11();

	grid.registerObstacle(10, 5);
	grid.registerObstacle(1, 5);
	grid.registerObstacle(5, 2);
	grid.registerObstacle(5, 7);

	return grid;
}

}