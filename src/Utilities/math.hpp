#pragma once

#include <SFML/Graphics.hpp>

#include <cmath>

namespace ph {

namespace Math 
{
	float getRightBound(const sf::FloatRect& bounds);
	float getBottomBound(const sf::FloatRect& bounds);

	sf::Vector2f getCenter(const sf::FloatRect& bounds);

	sf::Vector2f getTopLeftCorner(const sf::FloatRect& bounds);
	sf::Vector2f getTopRightCorner(const sf::FloatRect& bounds);
	sf::Vector2f getBottomLeftCorner(const sf::FloatRect& bounds);
	sf::Vector2f getBottomRightCorner(const sf::FloatRect& bounds);

	sf::Vector2f getUnitVector(float angle);
	float degreesToRadians(float angle);
	float radiansToDegrees(float angle);

	sf::Vector2u getTwoDimensionalPositionFromOneDimensionalArrayIndex(const unsigned index, const unsigned numberOfColumns);

	bool areTheyOverlapping(const sf::FloatRect& A, const sf::FloatRect& B);
	bool isPointInsideRect(const sf::Vector2f point, const sf::FloatRect& rect);
	bool isPointInsideCircle(const sf::Vector2f point, const sf::Vector2f circlePos, const float radius);

	sf::Vector2f lerp(const sf::Vector2f source, const sf::Vector2f destination, const float speed);

	template <typename T>
	float distanceBetweenPoints(const sf::Vector2<T>& point1, const sf::Vector2<T>& point2);
}
}

#include "math.inl"
