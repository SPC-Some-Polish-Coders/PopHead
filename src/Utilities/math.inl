#include "math.hpp"

template <typename T>
float distanceBetweenPoints(const sf::Vector2<T>& point1, const sf::Vector2<T>& point2)
{
	auto diff1 = point1.x - point2.x;
	auto diff2 = point1.y - point2.y;
	diff1 *= diff1;
	diff2 *= diff2;
	return std::sqrt(diff1 + diff2);
}
