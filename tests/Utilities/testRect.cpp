#include <catch.hpp>

#include "Utilities/rect.hpp"

TEST_CASE("Get bounds of rect", "[Utilities][Rect]")
{
	SECTION("Rect actualy is a point") {
		ph::FloatRect rect(0, 0, 0, 0);
		CHECK(rect.right() == 0);
		CHECK(rect.bottom() == 0);
		//CHECK(areEqual(Math::getCenter(rect), sf::Vector2f(0, 0)));
	}
	SECTION("Top left corner at (0, 0)") {
		ph::FloatRect rect(0, 0, 5, 8);
		CHECK(rect.right() == 5);
		CHECK(rect.bottom() == 8);
		//CHECK(Math::getCenter(rect) == sf::Vector2f(2.5, 4));
	}
	SECTION("Top left corner has positive coordinates") {
		ph::FloatRect rect(4, 9, 5, 8);
		CHECK(rect.right() == 4 + 5);
		CHECK(rect.bottom() == 9 + 8);
		//CHECK(areEqual(Math::getCenter(rect), sf::Vector2f(6.5f, 13.f)));
	}
	SECTION("Top left corner has negative coordinates") {
		ph::FloatRect rect(-3, -4, 3, 5);
		CHECK(rect.right() == -3 + 3);
		CHECK(rect.bottom() == -4 + 5);
		//CHECK(areEqual(Math::getCenter(rect), sf::Vector2f(-1.5f, -1.5f)));
	}
	SECTION("Top left corner has big coordinates") {
		ph::FloatRect rect(123000, -456710, 120, 6);
		CHECK(rect.right() == 123000 + 120);
		CHECK(rect.bottom() == -456710 + 6);
		//CHECK(areEqual(Math::getCenter(rect), sf::Vector2f(123060.f, -456707)));
	}
}

TEST_CASE("Get corners", "[Utilities][Math]")
{
	SECTION("Rect actualy is a point") {
		ph::FloatRect rect(0, 0, 0, 0);
		CHECK(rect.getTopLeft() == sf::Vector2f(0, 0));
		CHECK(rect.getTopRight() == sf::Vector2f(0, 0));
		CHECK(rect.getBottomLeft() == sf::Vector2f(0, 0));
		CHECK(rect.getBottomRight() == sf::Vector2f(0, 0));
	}
	SECTION("Top left corner at (0, 0)") {
		ph::FloatRect rect(0, 0, 5, 8);
		CHECK(rect.getTopLeft() == sf::Vector2f(0, 0));
		CHECK(rect.getTopRight() == sf::Vector2f(5, 0));
		CHECK(rect.getBottomLeft() == sf::Vector2f(0, 8));
		CHECK(rect.getBottomRight() == sf::Vector2f(5, 8));
	}
	SECTION("Top left corner has positive coordinates") {
		ph::FloatRect rect(5, 5, 2, 3);
		CHECK(rect.getTopLeft() == sf::Vector2f(5, 5));
		CHECK(rect.getTopRight() == sf::Vector2f(7, 5));
		CHECK(rect.getBottomLeft() == sf::Vector2f(5, 8));
		CHECK(rect.getBottomRight() == sf::Vector2f(7, 8));
	}
	SECTION("Top left corner has negative coordinates") {
		ph::FloatRect rect(-5, -5, 2, 3);
		CHECK(rect.getTopLeft() == sf::Vector2f(-5, -5));
		CHECK(rect.getTopRight() == sf::Vector2f(-3, -5));
		CHECK(rect.getBottomLeft() == sf::Vector2f(-5, -2));
		CHECK(rect.getBottomRight() == sf::Vector2f(-3, -2));
	}
	SECTION("Top left corner has big coordinates") {
		ph::FloatRect rect(123400.f, -98765430.f, 25.f, 30.f);
		CHECK(rect.getTopLeft() == sf::Vector2f(123400.f, -98765430.f));
		CHECK(rect.getTopRight() == sf::Vector2f(123425.f, -98765430.f));
		CHECK(rect.getBottomLeft() == sf::Vector2f(123400.f, -98765400.f));
		CHECK(rect.getBottomRight() == sf::Vector2f(123425.f, -98765400.f));
	}
}

TEST_CASE("Are they overlapping", "[Utilities][Math]")
{
	SECTION("Rects which are actually points and they are in the same place are not overlapping") {
		const ph::FloatRect rect(0, 0, 0, 0);
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(rect, rect));
	}
	SECTION("When rect which is actually a point is inside rect which is not a point, then they are overlapping") {
		const ph::FloatRect A(0, 0, 0, 0);
		const ph::FloatRect B(-2, -2, 4, 4);
		CHECK(ph::FloatRect::doPositiveRectsIntersect(A, B));
		CHECK(ph::FloatRect::doPositiveRectsIntersect(B, A));
	}
	SECTION("When rects are not points but they match, then they are overlapping") {
		const ph::FloatRect rect(-2, -2, 4, 4);
		CHECK(ph::FloatRect::doPositiveRectsIntersect(rect, rect));
	}
	SECTION("When A rect's right side is left of B rect's left side, then they are not overlapping") {
		const ph::FloatRect A(-3, 0, 2, 2);
		const ph::FloatRect B(0, 0, 2, 5);
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(A, B));
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(B, A));
	}
	SECTION("When A rect's left side is right of B rect's right side, then they are not overlapping") {
		const ph::FloatRect A(3, 0, 2, 2);
		const ph::FloatRect B(0, 0, 2, 5);
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(A, B));
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(B, A));
	}
	SECTION("When A rect's bottom side is up of B rect's up side, then they are not overlapping") {
		const ph::FloatRect A(0, -3, 2, 2);
		const ph::FloatRect B(0, 0, 5, 2);
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(A, B));
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(B, A));
	}
	SECTION("When A rect's up side is down of B rect's bottom side, then they are not overlapping") {
		const ph::FloatRect A(0, 3, 2, 2);
		const ph::FloatRect B(0, 0, 5, 2);
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(A, B));
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(B, A));
	}
	SECTION("A rect's right side touches B rect's left side") {
		const ph::FloatRect A(-2, 0, 2, 2);
		const ph::FloatRect B(0, -1, 5, 4);
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(A, B));
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(B, A));
	}
	SECTION("A rect's top side touches B rect's bottom side") {
		const ph::FloatRect A(0, 2, 1, 2);
		const ph::FloatRect B(0, -2, 5, 2);
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(A, B));
		CHECK_FALSE(ph::FloatRect::doPositiveRectsIntersect(B, A));
	}
}

TEST_CASE("Is point inside rect", "[Utilities][Math]")
{
	const ph::FloatRect rect(0, 0, 5, 5);
	CHECK(rect.containsIncludingBounds({0, 0}));
	CHECK(rect.containsIncludingBounds({2, 0}));
	CHECK(rect.containsIncludingBounds({0, 2}));
	CHECK(rect.containsIncludingBounds({2, 2}));
	CHECK(rect.containsIncludingBounds({5, 5}));
	CHECK_FALSE(rect.containsIncludingBounds({-2, 0}));
	CHECK_FALSE(rect.containsIncludingBounds({6, 0}));
	CHECK_FALSE(rect.containsIncludingBounds({0, -2}));
	CHECK_FALSE(rect.containsIncludingBounds({0, 6}));
}
