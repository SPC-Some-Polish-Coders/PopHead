#include "math.hpp"
#include "Logs/logs.hpp"

namespace ph {

namespace Math 
{
	float getRightBound(const sf::FloatRect& bounds)
	{
		return bounds.left + bounds.width;
	}

	float getBottomBound(const sf::FloatRect& bounds)
	{
		return bounds.top + bounds.height;
	}

	float getDistanceBetweenPoints(const sf::Vector2f firstPoint, const sf::Vector2f secondPoint)
	{
		return hypotf((secondPoint.x - firstPoint.x), (secondPoint.y - firstPoint.y));
	}

	sf::Vector2f getCenter(const sf::FloatRect& bounds)
	{
		return sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	}

	sf::Vector2f getTopLeftCorner(const sf::FloatRect& bounds)
	{
		return sf::Vector2f(bounds.left, bounds.top);
	}

	sf::Vector2f getTopRightCorner(const sf::FloatRect& bounds)
	{
		return sf::Vector2f(getRightBound(bounds), bounds.top);
	}

	sf::Vector2f getBottomLeftCorner(const sf::FloatRect& bounds)
	{
		return sf::Vector2f(bounds.left, getBottomBound(bounds));
	}

	sf::Vector2f getBottomRightCorner(const sf::FloatRect& bounds)
	{
		return sf::Vector2f(getRightBound(bounds), getBottomBound(bounds));
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

	bool areTheyOverlapping(const sf::FloatRect& A, const sf::FloatRect& B)
	{
		//AABB collision detection algorithm
		return
			A.left < getRightBound(B) &&
			getRightBound(A) > B.left &&
			A.top < getBottomBound(B) &&
			getBottomBound(A) > B.top;
	}

	bool isPointInsideRect(const sf::Vector2f point, const sf::FloatRect& rect)
	{
		return point.x >= rect.left && point.x <= rect.left + rect.width
			&& point.y >= rect.top && point.y <= rect.top + rect.height;
	}

	bool isPointInsideCircle(const sf::Vector2f point, const sf::Vector2f circlePos, const float radius)
	{
		return (getDistanceBetweenPoints(point, circlePos) < radius);
	}

	sf::Vector2f lerp(const sf::Vector2f source, const sf::Vector2f destination, const float speed)
	{
		PH_ASSERT(speed >= 0.f, "Speed cannot be less than 0");
		return source + (destination - source) * speed;
	}
}

}