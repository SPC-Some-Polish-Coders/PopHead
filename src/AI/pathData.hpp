#pragma once

#include <vector>
#include <deque>

namespace ph{

using ObstacleGrid = std::vector<std::vector<bool>>;

enum class Direction { east, west, north, south, northEast, northWest, southEast, southWest };
using Path = std::deque<Direction>;

}
