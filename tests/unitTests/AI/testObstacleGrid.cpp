#include <catch.hpp>

#include "AI/obstacleGrid.hpp"

namespace ph {

	TEST_CASE("ObstacleGrid is empty for default", "[AI][ObstacleGrid]")
	{
		ObstacleGrid grid;
		CHECK(grid.getColumnsCount() == 0);
		CHECK(grid.getRowsCount() == 0);
	}

	TEST_CASE("Empty ObstacleGrid is properly constructed by constructors", "[AI][ObstacleGrid]")
	{
		ObstacleGrid grid;

		SECTION("ctor1")
		{
			grid = ObstacleGrid(5, 4);
		}
		SECTION("ctor2")
		{
			std::vector<bool> arr(20, false);
			grid = ObstacleGrid(std::move(arr), 5, 4);
		}
		SECTION("ctor3")
		{
			std::vector<bool> column(4, false);
			std::vector<std::vector<bool>> map(5, column);
			grid = ObstacleGrid(std::move(map), 5, 4);
		}

		CHECK(grid.getColumnsCount() == 5);
		CHECK(grid.getRowsCount() == 4);

		for (size_t column = 0; column < 5; ++column)
			for (size_t row = 0; row < 4; ++row)
				CHECK(!grid.isObstacle(column, row));
	}

	TEST_CASE("ObstacleGrid with obstacles is properly constructed", "[AI][ObstacleGrid]")
	{
		ObstacleGrid grid;
		/* obstacles map
			10001
			01110
			10001
		*/
		std::vector<std::pair<size_t, size_t>> obstacles = {
			{0, 0},
			{4, 0},
			{1, 1},
			{2, 1},
			{3, 1},
			{0, 2},
			{4, 2}
		};

		SECTION("ctor1")
		{
			grid = ObstacleGrid(5, 3);
			for (const auto& pair : obstacles)
				grid.registerObstacle(pair.first, pair.second);
		}
		SECTION("ctor2")
		{
			std::vector<bool> arr{ 1,0,0,0,1,0,1,1,1,0,1,0,0,0,1 };
			grid = ObstacleGrid(std::move(arr), 5, 3);
		}
		SECTION("ctor3")
		{
			std::vector<bool> column1({ true, false, true });
			std::vector<bool> column2({ false, true, false });
			std::vector<bool> column3({ false, true, false });
			std::vector<bool> column4({ false, true, false });
			std::vector<bool> column5({ true, false, true });
			std::vector<std::vector<bool>> map{ column1, column2, column3, column4, column5 };
			grid = ObstacleGrid(std::move(map), 5, 3);
		}

		CHECK(grid.getColumnsCount() == 5);
		CHECK(grid.getRowsCount() == 3);

		for (size_t column = 0; column < 5; ++column)
			for (size_t row = 0; row < 3; ++row)
			{
				auto iter = std::find(obstacles.cbegin(), obstacles.cend(), std::make_pair(column, row));
				bool shouldBeObstacle = (iter != obstacles.cend());
				CHECK(grid.isObstacle(column, row) == shouldBeObstacle);
			}
	}

	TEST_CASE("Obstacles are properly registered", "[AI][ObstacleGrid]")
	{
		ObstacleGrid obstacles(10, 10);
		CHECK(obstacles.getColumnsCount() == 10);
		CHECK(obstacles.getRowsCount() == 10);

		for (size_t column = 0; column < 10; ++column)
			for (size_t row = 0; row < 10; ++row)
				CHECK(!obstacles.isObstacle(column, row));

		obstacles.registerObstacle(5, 5);
		obstacles.registerObstacle(3, 4);

		for (size_t column = 0; column < 10; ++column)
			for (size_t row = 0; row < 10; ++row)
			{
				bool shouldBeObstacle = false;
				if (column == 5 && row == 5)
					shouldBeObstacle = true;
				if (column == 3 && row == 4)
					shouldBeObstacle = true;

				CHECK(obstacles.isObstacle(column, row) == shouldBeObstacle);
			}
	}
}
