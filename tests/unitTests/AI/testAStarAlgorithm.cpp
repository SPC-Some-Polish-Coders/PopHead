#include <catch.hpp>

#include "AI/aStarAlgorithm.hpp"
#include "AI/obstacleGrid.hpp"

namespace ph {
	
	TEST_CASE("Grids with no obstacles", "[AI][AStarAlgorithm]")
	{
		SECTION("2x1 - straight line")
		{
			ObstacleGrid grid(2, 1);
			AStarAlgorithm aStar(grid, { 0, 0 }, { 1, 0 });
			auto path = aStar.getPath();
			CHECK(path.size() == 1);
			CHECK(path.at(0) == Direction::east);
		}
		SECTION("5x7 - straight line")
		{
			ObstacleGrid grid(5, 7);
			AStarAlgorithm aStar(grid, { 4, 3 }, { 0, 3 });
			auto path = aStar.getPath();
			CHECK(path.size() == 4);
			for (auto direction : path)
				CHECK(direction == Direction::west);
		}
		SECTION("2x2 - straight, diagonal line")
		{
			ObstacleGrid grid(2, 2);
			AStarAlgorithm aStar(grid, { 1, 0 }, { 0, 1 });
			auto path = aStar.getPath();
			CHECK(path.size() == 1);
			CHECK(path.at(0) == Direction::south_west);
		}
		SECTION("4x4 - straight, diagonal line")
		{
			ObstacleGrid grid(4, 4);
			AStarAlgorithm aStar(grid, { 3, 3 }, { 0, 0 });
			auto path = aStar.getPath();
			CHECK(path.size() == 3);
			for (auto direction : path)
				CHECK(direction == Direction::north_west);
		}
		SECTION("9x7 - straight, diagonal line")
		{
			ObstacleGrid grid(9, 7);
			AStarAlgorithm aStar(grid, { 0, 6 }, { 6, 0 });
			auto path = aStar.getPath();
			CHECK(path.size() == 6);
			for (auto direction : path)
				CHECK(direction == Direction::north_east);
		}
		
		SECTION("3x2 - two possibilities")
		{
			ObstacleGrid grid(3, 2);
			AStarAlgorithm aStar(grid, { 0, 0 }, { 2, 1 });
			auto path = aStar.getPath();
			CHECK(path.size() == 2);
			bool firstOption = (path.at(0) == Direction::south_east) && (path.at(1) == Direction::east);
			bool secondOption = (path.at(1) == Direction::south_east) && (path.at(0) == Direction::east);
			bool anyOption = firstOption || secondOption;
			CHECK(anyOption);
		}
	}

	TEST_CASE("Grids with obstacles, but without collisions", "[AI][AStarAlgorithm]")
	{
		SECTION("3x3")
		{
			ObstacleGrid grid(3, 3);
			grid.registerObstacle(0, 1);
			grid.registerObstacle(2, 1);
			AStarAlgorithm aStar(grid, { 1, 0 }, { 1, 2 });
			auto path = aStar.getPath();
			CHECK(path.size() == 2);
			CHECK(path.front() == Direction::south);
			path.pop_front();
			CHECK(path.front() == Direction::south);
		}
	}

	TEST_CASE("Grids with obstacles and collisions", "[AI][AStarAlgorithm]")
	{
		SECTION("2x3")
		{
			ObstacleGrid grid(2, 3);
			grid.registerObstacle(0, 1);
			SECTION("one avoidance")
			{
				AStarAlgorithm aStar(grid, { 0, 2 }, { 1, 0 });
				auto path = aStar.getPath();
				CHECK(path.size() == 3);
				CHECK(path.front() == Direction::east);
				path.pop_front();
				CHECK(path.front() == Direction::north);
				path.pop_front();
				CHECK(path.front() == Direction::north);
			}
			SECTION("two avoidances")
			{
				AStarAlgorithm aStar(grid, { 0, 2 }, { 0, 0 });
				auto path = aStar.getPath();
				CHECK(path.size() == 4);
				CHECK(path.front() == Direction::east);
				path.pop_front();
				CHECK(path.front() == Direction::north);
				path.pop_front();
				CHECK(path.front() == Direction::north);
				path.pop_front();
				CHECK(path.front() == Direction::west);
			}
		}
		SECTION("2x5")
		{
			ObstacleGrid grid(2, 5);
			grid.registerObstacle(0, 2);
			SECTION("one avoidance")
			{
				AStarAlgorithm aStar(grid, { 0, 4 }, { 1, 0 });
				auto path = aStar.getPath();
				CHECK(path.size() == 4);
				CHECK(path.front() == Direction::north_east);
				path.pop_front();
				CHECK(path.front() == Direction::north);
				path.pop_front();
				CHECK(path.front() == Direction::north);
				path.pop_front();
				CHECK(path.front() == Direction::north);
			}
			SECTION("two avoidances")
			{
				AStarAlgorithm aStar(grid, { 0, 4 }, { 0, 0 });
				auto path = aStar.getPath();
				CHECK(path.size() == 4);
				CHECK(path.front() == Direction::north_east);
				path.pop_front();
				CHECK(path.front() == Direction::north);
				path.pop_front();
				CHECK(path.front() == Direction::north);
				path.pop_front();
				CHECK(path.front() == Direction::north_west);
			}
		}
		SECTION("5x3")
		{
			ObstacleGrid grid(5, 3);
			grid.registerObstacle(1, 1);
			grid.registerObstacle(2, 1);
			grid.registerObstacle(3, 1);
			AStarAlgorithm aStar(grid, { 2, 2 }, { 1, 0 });
			auto path = aStar.getPath();
			CHECK(path.size() == 5);
			CHECK(path.front() == Direction::west);
			path.pop_front();
			CHECK(path.front() == Direction::west);
			path.pop_front();
			CHECK(path.front() == Direction::north);
			path.pop_front();
			CHECK(path.front() == Direction::north);
			path.pop_front();
			CHECK(path.front() == Direction::east);
		}
	}
}
