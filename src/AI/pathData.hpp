#pragma once

#include <vector>
#include <deque>

namespace ph{

using ObstacleGrids = std::vector<std::vector<bool>>;
enum class Direction
{
	north,
	north_east,
	east,
	south_east,
	south,
	south_west,
	west,
	north_west
};

using Path = std::deque<Direction>;

}
