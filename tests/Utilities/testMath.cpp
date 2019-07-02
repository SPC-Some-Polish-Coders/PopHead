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
	SECTION("rect is truly a point") {
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