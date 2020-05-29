#include "math.hpp"
#include "Logs/logs.hpp"

namespace ph::Math {

	template<typename T>
	Vec2Base<T> getUnitVector(Vec2Base<T> vec)
	{
		auto dist = distanceBetweenPoints(Vec2(), vec);
		return vec / dist;
	}

	template <typename T>
	Vec2Base<T> hadamardMul(Vec2Base<T> a, Vec2Base<T> b)
	{
		Vec2Base<T> res;
		res.x = a.x * b.x;
		res.y = a.y * b.y;
		return res;
	}

	template <typename T>
	Vec2Base<T> hadamardDiv(Vec2Base<T> a, Vec2Base<T> b)
	{
		Vec2Base<T> res;
		res.x = a.x / b.x;
		res.y = a.y / b.y;
		return res;
	}

	template <typename T>
	float distanceBetweenPoints(Vec2Base<T> point1, Vec2Base<T> point2)
	{
		auto diff1 = point1.x - point2.x;
		auto diff2 = point1.y - point2.y;
		diff1 *= diff1;
		diff2 *= diff2;
		return Cast<float>(std::sqrt(diff1 + diff2));
	}

	template<typename T>
	Vec2Base<T> abs(Vec2Base<T> vec)
	{
		return Vec2Base<T>(std::abs(vec.x), std::abs(vec.y));
	}

	Vec2 getUnitVector(float angle)
	{
		auto radians = degreesToRadians(angle);
		return Vec2(std::cosf(radians), std::sinf(radians));
	}

	float getMagnitude(Vec2 v)
	{
		return std::hypotf(v.x, v.y);	
	}

	float degreesToRadians(float angle)
	{
		return angle * pi / 180.f;
	}

	float radiansToDegrees(float angle)
	{
		return angle * 180.f / pi;
	}

	Vec2u getTwoDimensionalPositionFromOneDimensionalArrayIndex(u32 index, u32 numberOfColumns)
	{
		PH_ASSERT_UNEXPECTED_SITUATION(numberOfColumns != 0, "Number of columns cannot be 0");
		return Vec2u(index % numberOfColumns, index / numberOfColumns);
	}

	bool isPointInsideCircle(Vec2 point, Vec2 circlePos, float radius)
	{
		return distanceBetweenPoints(point, circlePos) < radius;
	}

	bool intersect(FloatRect rect, Vec2 circlePos, float radius)
	{
		return rect.contains(circlePos) || 
		       isPointInsideCircle(rect.pos, circlePos, radius) ||
		       isPointInsideCircle(rect.topRight(), circlePos, radius) ||
		       isPointInsideCircle(rect.bottomLeft(), circlePos, radius) ||
		       isPointInsideCircle(rect.bottomRight(), circlePos, radius) ||
		       isPointInsideCircle(rect.topCenter(), circlePos, radius) ||
		       isPointInsideCircle(rect.bottomCenter(), circlePos, radius) ||
		       isPointInsideCircle(rect.leftCenter(), circlePos, radius) ||
		       isPointInsideCircle(rect.rightCenter(), circlePos, radius); 
	}

	Vec2 lerp(Vec2 source, Vec2 destination, float speed)
	{
		PH_ASSERT_UNEXPECTED_SITUATION(speed >= 0.f, "Speed cannot be less than 0");
		return source + (destination - source) * speed;
	}

	float lerp(float source, float destination, float speed)
	{
		PH_ASSERT_UNEXPECTED_SITUATION(speed >= 0.f, "Speed cannot be less than 0");
		return source + (destination - source) * speed;
	}

	bool areApproximatelyEqual(float a, float b, float maxApproximation)
	{
		PH_ASSERT_UNEXPECTED_SITUATION(maxApproximation >= 0.f, "max approximation has to be positive value");
		return (std::fabs(a - b) < maxApproximation);
	}

	bool areApproximatelyEqual(Vec2 a, Vec2 b, float maxApproximation)
	{
		return areApproximatelyEqual(a.x, b.x, maxApproximation) && areApproximatelyEqual(a.y, b.y, maxApproximation);
	}

}
