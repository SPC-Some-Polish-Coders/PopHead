#include "catch.hpp"

#include "Utilities/math.hpp"

namespace ph {

namespace {
	bool areEqual(const float f1, const float f2)
	{
		return Approx(f2).margin(0.001f) == f1;
	}

	bool areEqual(const sf::Vector2f v1, const sf::Vector2f v2)
	{
		return areEqual(v1.x, v2.x) && areEqual(v1.y, v2.y);
	}
}

TEST_CASE("Get bounds and center", "[Utilities][Math]")
{
	SECTION("Rect actualy is a point") {
		sf::FloatRect rect(0, 0, 0, 0);
		CHECK(Math::getRightBound(rect) == 0);
		CHECK(Math::getBottomBound(rect) == 0);
		CHECK(areEqual(Math::getCenter(rect), sf::Vector2f(0, 0)));
	}
	SECTION("Top left corner at (0, 0)") {
		sf::FloatRect rect(0, 0, 5, 8);
		CHECK(Math::getRightBound(rect) == 5);
		CHECK(Math::getBottomBound(rect) == 8);
		CHECK(Math::getCenter(rect) == sf::Vector2f(2.5, 4));
	}
	SECTION("Top left corner has positive coordinates") {
		sf::FloatRect rect(4, 9, 5, 8);
		CHECK(Math::getRightBound(rect) == 4 + 5);
		CHECK(Math::getBottomBound(rect) == 9 + 8);
		CHECK(areEqual(Math::getCenter(rect), sf::Vector2f(6.5f, 13.f)));
	}
	SECTION("Top left corner has negative coordinates") {
		sf::FloatRect rect(-3, -4, 3, 5);
		CHECK(Math::getRightBound(rect) == -3 + 3);
		CHECK(Math::getBottomBound(rect) == -4 + 5);
		CHECK(areEqual(Math::getCenter(rect), sf::Vector2f(-1.5f, -1.5f)));
	}
	SECTION("Top left corner has big coordinates") {
		sf::FloatRect rect(123000, -456710, 120, 6);
		CHECK(Math::getRightBound(rect) == 123000 + 120);
		CHECK(Math::getBottomBound(rect) == -456710 + 6);
		CHECK(areEqual(Math::getCenter(rect), sf::Vector2f(123060.f, -456707)));
	}
}

TEST_CASE("Get corners", "[Utilities][Math]")
{
	SECTION("Rect actualy is a point") {
		sf::FloatRect rect(0, 0, 0, 0);
		CHECK(Math::getTopLeftCorner(rect) == sf::Vector2f(0, 0));
		CHECK(Math::getTopRightCorner(rect) == sf::Vector2f(0, 0));
		CHECK(Math::getBottomLeftCorner(rect) == sf::Vector2f(0, 0));
		CHECK(Math::getBottomRightCorner(rect) == sf::Vector2f(0, 0));
	}
	SECTION("Top left corner at (0, 0)") {
		sf::FloatRect rect(0, 0, 5, 8);
		CHECK(Math::getTopLeftCorner(rect) == sf::Vector2f(0, 0));
		CHECK(Math::getTopRightCorner(rect) == sf::Vector2f(5, 0));
		CHECK(Math::getBottomLeftCorner(rect) == sf::Vector2f(0, 8));
		CHECK(Math::getBottomRightCorner(rect) == sf::Vector2f(5, 8));
	}
	SECTION("Top left corner has positive coordinates") {
		sf::FloatRect rect(5, 5, 2, 3);
		CHECK(Math::getTopLeftCorner(rect) == sf::Vector2f(5, 5));
		CHECK(Math::getTopRightCorner(rect) == sf::Vector2f(7, 5));
		CHECK(Math::getBottomLeftCorner(rect) == sf::Vector2f(5, 8));
		CHECK(Math::getBottomRightCorner(rect) == sf::Vector2f(7, 8));
	}
	SECTION("Top left corner has negative coordinates") {
		sf::FloatRect rect(-5, -5, 2, 3);
		CHECK(Math::getTopLeftCorner(rect) == sf::Vector2f(-5, -5));
		CHECK(Math::getTopRightCorner(rect) == sf::Vector2f(-3, -5));
		CHECK(Math::getBottomLeftCorner(rect) == sf::Vector2f(-5, -2));
		CHECK(Math::getBottomRightCorner(rect) == sf::Vector2f(-3, -2));
	}
	SECTION("Top left corner has big coordinates") {
		sf::FloatRect rect(123400.f, -98765430.f, 25.f, 30.f);
		CHECK(Math::getTopLeftCorner(rect) == sf::Vector2f(123400.f, -98765430.f));
		CHECK(Math::getTopRightCorner(rect) == sf::Vector2f(123425.f, -98765430.f));
		CHECK(Math::getBottomLeftCorner(rect) == sf::Vector2f(123400.f, -98765400.f));
		CHECK(Math::getBottomRightCorner(rect) == sf::Vector2f(123425.f, -98765400.f));
	}
}

TEST_CASE("Get two dimensional position from one dimensional array index", "[Utilities][Math]")
{
	// These sections are commented because ini loader thrown exceptions and these tests did not work correcly.
	// Uncomment these tests when the logs are cleared.
	/*SECTION("index is 0 and array size is 0") {
		CHECK_THROWS_WITH(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(0, 0), "Number of columns cannot be 0");
	}
	SECTION("index is positive and number of columns is 0") {
		CHECK_THROWS_WITH(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(5, 0), "Number of columns cannot be 0");
	}*/
	SECTION("Index is 0 and number of columns is positive") {
		CHECK(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(0, 5) == sf::Vector2u(0, 0));
	}
	SECTION("Index and number of columns are positive but index is lesser then number of columns") {
		CHECK(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(3, 5) == sf::Vector2u(3, 0));
	}
	SECTION("Index and number of columns are positive and they match") {
		CHECK(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(5, 5) == sf::Vector2u(0, 1));
	}
	SECTION("Index is bigger then number of columns") {
		CHECK(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(12, 5) == sf::Vector2u(2, 2));
	}
}

TEST_CASE("Are they overlapping", "[Utilities][Math]")
{
	SECTION("Rects which are actually points and they are in the same place are not overlapping") {
		const sf::FloatRect rect(0, 0, 0, 0);
		CHECK_FALSE(Math::areTheyOverlapping(rect, rect));
	}
	SECTION("When rect which is actually a point is inside rect which is not a point, then they are overlapping") {
		const sf::FloatRect A(0, 0, 0, 0);
		const sf::FloatRect B(-2, -2, 4, 4);
		CHECK(Math::areTheyOverlapping(A, B));
		CHECK(Math::areTheyOverlapping(B, A));
	}
	SECTION("When rects are not points but they match, then they are overlapping") {
		const sf::FloatRect rect(-2, -2, 4, 4);
		CHECK(Math::areTheyOverlapping(rect, rect));
	}
	SECTION("When A rect's right side is left of B rect's left side, then they are not overlapping") {
		const sf::FloatRect A(-3, 0, 2, 2);
		const sf::FloatRect B(0, 0, 2, 5);
		CHECK_FALSE(Math::areTheyOverlapping(A, B));
		CHECK_FALSE(Math::areTheyOverlapping(B, A));
	}
	SECTION("When A rect's left side is right of B rect's right side, then they are not overlapping") {
		const sf::FloatRect A(3, 0, 2, 2);
		const sf::FloatRect B(0, 0, 2, 5);
		CHECK_FALSE(Math::areTheyOverlapping(A, B));
		CHECK_FALSE(Math::areTheyOverlapping(B, A));
	}
	SECTION("When A rect's bottom side is up of B rect's up side, then they are not overlapping") {
		const sf::FloatRect A(0, -3, 2, 2);
		const sf::FloatRect B(0, 0, 5, 2);
		CHECK_FALSE(Math::areTheyOverlapping(A, B));
		CHECK_FALSE(Math::areTheyOverlapping(B, A));
	}
	SECTION("When A rect's up side is down of B rect's bottom side, then they are not overlapping") {
		const sf::FloatRect A(0, 3, 2, 2);
		const sf::FloatRect B(0, 0, 5, 2);
		CHECK_FALSE(Math::areTheyOverlapping(A, B));
		CHECK_FALSE(Math::areTheyOverlapping(B, A));
	}
	SECTION("A rect's right side touches B rect's left side") {
		const sf::FloatRect A(-2, 0, 2, 2);
		const sf::FloatRect B(0, -1, 5, 4);
		CHECK_FALSE(Math::areTheyOverlapping(A, B));
		CHECK_FALSE(Math::areTheyOverlapping(B, A));
	}
	SECTION("A rect's top side touches B rect's bottom side") {
		const sf::FloatRect A(0, 2, 1, 2);
		const sf::FloatRect B(0, -2, 5, 2);
		CHECK_FALSE(Math::areTheyOverlapping(A, B));
		CHECK_FALSE(Math::areTheyOverlapping(B, A));
	}
}

TEST_CASE("Is point inside rect", "[Utilities][Math]")
{
	const sf::FloatRect rect(0, 0, 5, 5);
	CHECK(Math::isPointInsideRect({0, 0}, rect));
	CHECK(Math::isPointInsideRect({2, 0}, rect));
	CHECK(Math::isPointInsideRect({0, 2}, rect));
	CHECK(Math::isPointInsideRect({2, 2}, rect));
	CHECK(Math::isPointInsideRect({5, 5}, rect));
	CHECK_FALSE(Math::isPointInsideRect({-2, 0}, rect));
	CHECK_FALSE(Math::isPointInsideRect({6, 0}, rect));
	CHECK_FALSE(Math::isPointInsideRect({0, -2}, rect));
	CHECK_FALSE(Math::isPointInsideRect({0, 6}, rect));
}

TEST_CASE("Lerp", "[Utilities][Math]")
{
	// These section is commented because ini loader thrown exceptions and these tests did not work correcly.
	// Uncomment this test when the logs are cleared.
	/*SECTION("index is 0 and array size is 0") {
		CHECK_THROWS_WITH(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(0, 0), "Number of columns cannot be 0");
	}*/
	SECTION("For positive source and destination") {
		const sf::Vector2f source(5, 5);
		const sf::Vector2f destination(10, 10);
		{
			const float speed = 2.f;
			CHECK(areEqual(Math::lerp(source, destination, speed), {15, 15}));
		}
		{
			const float speed = 1.f;
			CHECK(areEqual(Math::lerp(source, destination, speed), {10, 10}));
		}
		{
			const float speed = 0.5f;
			CHECK(areEqual(Math::lerp(source, destination, speed), {7.5, 7.5}));
		}
		{
			const float speed = 0.25f;
			CHECK(areEqual(Math::lerp(source, destination, speed), {6.25, 6.25}));
		}
	}
	SECTION("For negative source and destination") {
		const sf::Vector2f source(-5, -5);
		const sf::Vector2f destination(-10, -10);
		const float speed = 0.5;
		const sf::Vector2f result = Math::lerp(source, destination, speed);
		CHECK(areEqual(result, {-7.5, -7.5}));
	}
	SECTION("When speed is 0.5, result equals (0, 0) when source and destination are negative values") {
		const sf::Vector2f source(5, 5);
		const sf::Vector2f destination(-5, -5);
		const float speed = 0.5;
		SECTION("For positive source and negative destination") {
			const sf::Vector2f result = Math::lerp(source, destination, speed);
			CHECK(areEqual(result, {0, 0}));
		}
		SECTION("For negative source and positive destination") {
			const sf::Vector2f result = Math::lerp(-source, -destination, speed);
			CHECK(areEqual(result, {0, 0}));
		}
	}
}

TEMPLATE_TEST_CASE("Distance between two points", "[Utilities][Math][distanceBetweenPoints]", int, unsigned, float, double)
{
	SECTION("Distance equal 0")
	{
		SECTION("1:")
		{
			sf::Vector2<TestType> point1(4, 5);
			sf::Vector2<TestType> point2(4, 5);
			float distance = Math::distanceBetweenPoints(point1, point2);
			CHECK(distance == Approx(0).margin(0.00001));
		}
		SECTION("2:")
		{
			sf::Vector2<TestType> point1(0, 0);
			sf::Vector2<TestType> point2(0, 0);
			float distance = Math::distanceBetweenPoints(point1, point2);
			CHECK(distance == Approx(0).margin(0.00001));
		}
		SECTION("3:")
		{
			sf::Vector2<TestType> point1(-8, 7);
			sf::Vector2<TestType> point2(-8, 7);
			float distance = Math::distanceBetweenPoints(point1, point2);
			CHECK(distance == Approx(0).margin(0.00001));
		}
	}
	SECTION("Distance is greater than 0")
	{
		SECTION("1:")
		{
			sf::Vector2<TestType> point1(0, 0);
			sf::Vector2<TestType> point2(3, 4);
			float distance = Math::distanceBetweenPoints(point1, point2);
			CHECK(distance == Approx(5).margin(0.00001));
		}
		SECTION("2:")
		{
			sf::Vector2<TestType> point1(-2, -2);
			sf::Vector2<TestType> point2(1, 2);
			float distance = Math::distanceBetweenPoints(point1, point2);
			CHECK(distance == Approx(5).margin(0.00001));
		}
		SECTION("3:")
		{
			sf::Vector2<TestType> point1(-3, 5);
			sf::Vector2<TestType> point2(-3, 14);
			float distance = Math::distanceBetweenPoints(point1, point2);
			CHECK(distance == Approx(9).margin(0.00001));
		}
		SECTION("4:")
		{
			sf::Vector2<TestType> point1(-8, 7);
			sf::Vector2<TestType> point2(15, 29);
			float distance = Math::distanceBetweenPoints(point1, point2);
			CHECK(distance == Approx(31.82766).margin(0.00001));
		}
	}
}

}
