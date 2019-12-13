#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <cmath>

namespace ph::Math {

	template <typename T>
	sf::Vector2<T> getUnitVector(const sf::Vector2<T>);

	sf::Vector2f getUnitVector(float angle);
	
	float degreesToRadians(float angle);
	float radiansToDegrees(float angle);

	sf::Vector2u getTwoDimensionalPositionFromOneDimensionalArrayIndex(const unsigned index, const unsigned numberOfColumns);

	bool isPointInsideCircle(const sf::Vector2f point, const sf::Vector2f circlePos, const float radius);

	sf::Vector2f lerp(const sf::Vector2f source, const sf::Vector2f destination, const float speed);

	template <typename T>
	float distanceBetweenPoints(const sf::Vector2<T>& point1, const sf::Vector2<T>& point2);

	template <typename T>
	sf::Vector2<T> abs(const sf::Vector2<T>);

	bool areApproximatelyEqual(float a, float b, float maxApproximation);
	bool areApproximatelyEqual(const sf::Vector2f a, const sf::Vector2f b, float maxApproximation);
}

#include "math.inl"
