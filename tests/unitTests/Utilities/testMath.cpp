#include "catch.hpp"

#include "Utilities/math.hpp"

namespace ph {

namespace {
	bool areEqual(const float f1, const float f2)
	{
		return Approx(f2).margin(0.001f) == f1;
	}

	bool areEqual(const Vec2 v1, const Vec2 v2)
	{
		return areEqual(v1.x, v2.x) && areEqual(v1.y, v2.y);
	}
}

TEST_CASE("Get two dimensional position from one dimensional array index", "[Utilities][Math]")
{
	// These sections are commented because ini loader thrown exceptions and these tests did not work correctly.
	// Uncomment these tests when the logs are cleared.
	/*SECTION("index is 0 and array size is 0") {
		CHECK_THROWS_WITH(getTwoDimensionalPositionFromOneDimensionalArrayIndex(0, 0), "Number of columns cannot be 0");
	}
	SECTION("index is positive and number of columns is 0") {
		CHECK_THROWS_WITH(getTwoDimensionalPositionFromOneDimensionalArrayIndex(5, 0), "Number of columns cannot be 0");
	}*/
	SECTION("Index is 0 and number of columns is positive") {
		CHECK(getTwoDimensionalPositionFromOneDimensionalArrayIndex(0, 5) == Vec2u(0, 0));
	}
	SECTION("Index and number of columns are positive but index is lesser then number of columns") {
		CHECK(getTwoDimensionalPositionFromOneDimensionalArrayIndex(3, 5) == Vec2u(3, 0));
	}
	SECTION("Index and number of columns are positive and they match") {
		CHECK(getTwoDimensionalPositionFromOneDimensionalArrayIndex(5, 5) == Vec2u(0, 1));
	}
	SECTION("Index is bigger then number of columns") {
		CHECK(getTwoDimensionalPositionFromOneDimensionalArrayIndex(12, 5) == Vec2u(2, 2));
	}
}

TEST_CASE("Lerp", "[Utilities][Math]")
{
	// These section is commented because ini loader thrown exceptions and these tests did not work correcly.
	// Uncomment this test when the logs are cleared.
	/*SECTION("index is 0 and array size is 0") {
		CHECK_THROWS_WITH(getTwoDimensionalPositionFromOneDimensionalArrayIndex(0, 0), "Number of columns cannot be 0");
	}*/
	SECTION("For positive source and destination") {
		const Vec2 source(5, 5);
		const Vec2 destination(10, 10);
		{
			const float speed = 2.f;
			CHECK(areEqual(lerp(source, destination, speed), {15, 15}));
		}
		{
			const float speed = 1.f;
			CHECK(areEqual(lerp(source, destination, speed), {10, 10}));
		}
		{
			const float speed = 0.5f;
			CHECK(areEqual(lerp(source, destination, speed), {7.5, 7.5}));
		}
		{
			const float speed = 0.25f;
			CHECK(areEqual(lerp(source, destination, speed), {6.25, 6.25}));
		}
	}
	SECTION("For negative source and destination") {
		const Vec2 source(-5, -5);
		const Vec2 destination(-10, -10);
		const float speed = 0.5;
		const Vec2 result = lerp(source, destination, speed);
		CHECK(areEqual(result, {-7.5, -7.5}));
	}
	SECTION("When speed is 0.5, result equals (0, 0) when source and destination are negative values") {
		const Vec2 source(5, 5);
		const Vec2 destination(-5, -5);
		const float speed = 0.5;
		SECTION("For positive source and negative destination") {
			const Vec2 result = lerp(source, destination, speed);
			CHECK(areEqual(result, {0, 0}));
		}
		SECTION("For negative source and positive destination") {
			const Vec2 result = lerp(-source, -destination, speed);
			CHECK(areEqual(result, {0, 0}));
		}
	}
}

TEMPLATE_TEST_CASE("Distance between two points", "[Utilities][Math][distanceBetweenpoints]", i32, u32, float, double)
{
	SECTION("Distance equal 0")
	{
		SECTION("1:")
		{
			sf::Vector2<TestType> point1(4, 5);
			sf::Vector2<TestType> point2(4, 5);
			float distance = distanceBetweenpoints(point1, point2);
			CHECK(distance == Approx(0).margin(0.00001));
		}
		SECTION("2:")
		{
			sf::Vector2<TestType> point1(0, 0);
			sf::Vector2<TestType> point2(0, 0);
			float distance = distanceBetweenpoints(point1, point2);
			CHECK(distance == Approx(0).margin(0.00001));
		}
		SECTION("3:")
		{
			sf::Vector2<TestType> point1(-8, 7);
			sf::Vector2<TestType> point2(-8, 7);
			float distance = distanceBetweenpoints(point1, point2);
			CHECK(distance == Approx(0).margin(0.00001));
		}
	}
	SECTION("Distance is greater than 0")
	{
		SECTION("1:")
		{
			sf::Vector2<TestType> point1(0, 0);
			sf::Vector2<TestType> point2(3, 4);
			float distance = distanceBetweenpoints(point1, point2);
			CHECK(distance == Approx(5).margin(0.00001));
		}
		SECTION("2:")
		{
			sf::Vector2<TestType> point1(-2, -2);
			sf::Vector2<TestType> point2(1, 2);
			float distance = distanceBetweenpoints(point1, point2);
			CHECK(distance == Approx(5).margin(0.00001));
		}
		SECTION("3:")
		{
			sf::Vector2<TestType> point1(-3, 5);
			sf::Vector2<TestType> point2(-3, 14);
			float distance = distanceBetweenpoints(point1, point2);
			CHECK(distance == Approx(9).margin(0.00001));
		}
		SECTION("4:")
		{
			sf::Vector2<TestType> point1(-8, 7);
			sf::Vector2<TestType> point2(15, 29);
			float distance = distanceBetweenpoints(point1, point2);
			CHECK(distance == Approx(31.82766).margin(0.00001));
		}
	}
}

}
