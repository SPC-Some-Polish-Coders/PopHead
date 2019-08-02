#pragma once

#include <vector>
#include <deque>

namespace ph{

using ObstacleGrid = std::vector<std::vector<bool>>;

enum class Direction { east, west, north, south, none };
using Path = std::deque<Direction>;

}
