#pragma once

#include <vector>
#include <deque>

namespace ph{

enum class Direction
{
	north,
	north_east,
	east,
	south_east,
	south,
	south_west,
	west,
	north_west,
	none
};

using Path = std::deque<Direction>;

}
