#include "math.hpp"
#include "Logs/logs.hpp"

namespace ph::Math {

	template<typename T>
	sf::Vector2<T> getUnitVector(const sf::Vector2<T> vec)
	{
		auto dist = distanceBetweenPoints(sf::Vector2f(), vec);
		return vec / dist;
	}

	template <typename T>
	float distanceBetweenPoints(const sf::Vector2<T>& point1, const sf::Vector2<T>& point2)
	{
		auto diff1 = point1.x - point2.x;
		auto diff2 = point1.y - point2.y;
		diff1 *= diff1;
		diff2 *= diff2;
		return static_cast<float>(std::sqrt(diff1 + diff2));
	}

	template<typename T>
	sf::Vector2<T> abs(const sf::Vector2<T> vec)
	{
		return sf::Vector2<T>(std::abs(vec.x), std::abs(vec.y));
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
		PH_ASSERT_UNEXPECTED_SITUATION(numberOfColumns != 0, "Number of columns cannot be 0");
		return sf::Vector2u(index % numberOfColumns, index / numberOfColumns);
	}

	bool isPointInsideCircle(const sf::Vector2f point, const sf::Vector2f circlePos, const float radius)
	{
		return (distanceBetweenPoints(point, circlePos) < radius);
	}

	sf::Vector2f lerp(const sf::Vector2f source, const sf::Vector2f destination, const float speed)
	{
		PH_ASSERT_UNEXPECTED_SITUATION(speed >= 0.f, "Speed cannot be less than 0");
		return source + (destination - source) * speed;
	}

	bool areApproximatelyEqual(float a, float b, float maxApproximation)
	{
		PH_ASSERT_UNEXPECTED_SITUATION(maxApproximation >= 0.f, "max approximation has to be positive value");
		return (std::fabs(a - b) < maxApproximation);
	}

	bool areApproximatelyEqual(const sf::Vector2f a, const sf::Vector2f b, float maxApproximation)
	{
		return areApproximatelyEqual(a.x, b.x, maxApproximation) && areApproximatelyEqual(a.y, b.y, maxApproximation);
	}

}
