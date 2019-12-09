#include "math.hpp"
#include "Logs/logs.hpp"

namespace ph::Math {

	float getRightBound(const sf::FloatRect& bounds)
	{
		return bounds.left + bounds.width;
	}

	float getBottomBound(const sf::FloatRect& bounds)
	{
		return bounds.top + bounds.height;
	}

	sf::Vector2f getUnitVector(float angle)
	{
		auto radians = degreesToRadians(angle);
		return sf::Vector2f(std::cosf(radians), std::sinf(radians));
	}

	float degreesToRadians(float angle)
	{
		return angle * 3.14159f / 180.f;
	}

	float radiansToDegrees(float angle)
	{
		return angle * 180.f / 3.14159f;
	}

	sf::Vector2u getTwoDimensionalPositionFromOneDimensionalArrayIndex(const unsigned index, const unsigned numberOfColumns)
	{
		PH_ASSERT_EXCEPTION(numberOfColumns != 0, "Number of columns cannot be 0");
		return sf::Vector2u(index % numberOfColumns, index / numberOfColumns);
	}

	bool isPointInsideCircle(const sf::Vector2f point, const sf::Vector2f circlePos, const float radius)
	{
		return (distanceBetweenPoints(point, circlePos) < radius);
	}

	sf::Vector2f lerp(const sf::Vector2f source, const sf::Vector2f destination, const float speed)
	{
		PH_ASSERT(speed >= 0.f, "Speed cannot be less than 0");
		return source + (destination - source) * speed;
	}
}
