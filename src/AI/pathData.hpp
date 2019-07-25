#pragma once

#include <vector>
#include <deque>

namespace ph{

enum class AreaType { walkable, obstacle };
using Grid = std::vector<std::vector<AreaType>>;

enum class Direction { east, west, north, south, northEast, northWest, southEast, southWest };
using Path = std::deque<Direction>;

}
