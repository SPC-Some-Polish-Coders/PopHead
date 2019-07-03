#include "catch.hpp"

#include "Utilities/math.hpp"
#include <iostream>

using namespace ph;

static bool areEqual(float f1, float f2)
{
	return Approx(f2).margin(0.001f) == f1;
}

static bool areEqual(sf::Vector2f v1, sf::Vector2f v2)
{
	return areEqual(v1.x, v2.x) && areEqual(v1.y, v2.y);
}

TEST_CASE("get bounds and center", "[Utilities][Math]")
{
	SECTION("rect actualy is a point") {
		sf::FloatRect rect(0, 0, 0, 0);
		CHECK(Math::getRightBound(rect) == 0);
		CHECK(Math::getBottomBound(rect) == 0);
		CHECK(areEqual(Math::getCenter(rect), sf::Vector2f(0, 0)));
	}
	SECTION("top left corner at (0, 0)") {
		sf::FloatRect rect(0, 0, 5, 8);
		CHECK(Math::getRightBound(rect) == 5);
		CHECK(Math::getBottomBound(rect) == 8);
		CHECK(Math::getCenter(rect) == sf::Vector2f(2.5, 4));
	}
	SECTION("top left corner has positive coordinates") {
		sf::FloatRect rect(4, 9, 5, 8);
		CHECK(Math::getRightBound(rect) == 4 + 5);
		CHECK(Math::getBottomBound(rect) == 9 + 8);
		CHECK(areEqual(Math::getCenter(rect), sf::Vector2f(6.5f, 13.f)));
	}
	SECTION("top left corner has negative coordinates") {
		sf::FloatRect rect(-3, -4, 3, 5);
		CHECK(Math::getRightBound(rect) == -3 + 3);
		CHECK(Math::getBottomBound(rect) == -4 + 5);
		CHECK(areEqual(Math::getCenter(rect), sf::Vector2f(-1.5f, -1.5f)));
	}
	SECTION("top left corner has big coordinates") {
		sf::FloatRect rect(123000, -456710, 120, 6);
		CHECK(Math::getRightBound(rect) == 123000 + 120);
		CHECK(Math::getBottomBound(rect) == -456710 + 6);
		CHECK(areEqual(Math::getCenter(rect), sf::Vector2f(123060.f, -456707)));
	}
}

TEST_CASE("get corners", "[Utilities][Math]")
{
	SECTION("rect actualy is a point") {
		sf::FloatRect rect(0, 0, 0, 0);
		CHECK(Math::getTopRightCorner(rect) == sf::Vector2f(0, 0));
		CHECK(Math::getBottomLeftCorner(rect) == sf::Vector2f(0, 0));
		CHECK(Math::getBottomRightCorner(rect) == sf::Vector2f(0, 0));
	}
	SECTION("top left corner at (0, 0)") {
		sf::FloatRect rect(0, 0, 5, 8);
		CHECK(Math::getTopRightCorner(rect) == sf::Vector2f(5, 0));
		CHECK(Math::getBottomLeftCorner(rect) == sf::Vector2f(0, 8));
		CHECK(Math::getBottomRightCorner(rect) == sf::Vector2f(5, 8));
	}
	SECTION("top left corner has positive coordinates") {
		sf::FloatRect rect(5, 5, 2, 3);
		CHECK(Math::getTopRightCorner(rect) == sf::Vector2f(7, 5));
		CHECK(Math::getBottomLeftCorner(rect) == sf::Vector2f(5, 8));
		CHECK(Math::getBottomRightCorner(rect) == sf::Vector2f(7, 8));
	}
	SECTION("top left corner has negative coordinates") {
		sf::FloatRect rect(-5, -5, 2, 3);
		CHECK(Math::getTopRightCorner(rect) == sf::Vector2f(-3, -5));
		CHECK(Math::getBottomLeftCorner(rect) == sf::Vector2f(-5, -2));
		CHECK(Math::getBottomRightCorner(rect) == sf::Vector2f(-3, -2));
	}
	SECTION("top left corner has big coordinates") {
		sf::FloatRect rect(123400, -98765430, 25, 30);
		CHECK(Math::getTopRightCorner(rect) == sf::Vector2f(123425, -98765430));
		CHECK(Math::getBottomLeftCorner(rect) == sf::Vector2f(123400, -98765400));
		CHECK(Math::getBottomRightCorner(rect) == sf::Vector2f(123425, -98765400));
	}
}

TEST_CASE("get two dimensional position from one dimensional array index", "[Utilities][Math]")
{
	// TODO: Make commented assertions work!
	/*SECTION("index is 0 and array size is 0") {
		CHECK_THROWS(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(0, 0));
	}*/
	/*SECTION("index is positive and number of columns is 0") {
		CHECK_THROWS(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(5, 0));
	}*/
	SECTION("index is 0 and number of columns is positive") {
		CHECK(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(0, 5) == sf::Vector2u(0, 0));
	}
	SECTION("index and number of columns are positive but index is lesser then number of columns") {
		CHECK(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(3, 5) == sf::Vector2u(3, 0));
	}
	SECTION("index and number of columns are positive and they match") {
		CHECK(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(5, 5) == sf::Vector2u(0, 1));
	}
	SECTION("index is bigger then number of columns") {
		CHECK(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(12, 5) == sf::Vector2u(2, 2));
	}
}