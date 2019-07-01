#pragma once

#include "Utilities/debug.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

namespace Math {
	inline float getRightBound(sf::FloatRect bounds)
	{ return bounds.left + bounds.width; }

	inline float getBottomBound(sf::FloatRect bounds)  
	{ return bounds.top + bounds.height; }

	inline sf::Vector2f getCenter(sf::FloatRect bounds)
	{ return sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f); }

	inline sf::Vector2f getTopRightCorner(sf::FloatRect bounds)
	{ return sf::Vector2f(getRightBound(bounds), bounds.top); }

	inline sf::Vector2f getBottomLeftCorner(sf::FloatRect bounds)
	{ return sf::Vector2f(bounds.left, getBottomBound(bounds)); }

	inline sf::Vector2f getBottomRightCorner(sf::FloatRect bounds)
	{ return sf::Vector2f(getRightBound(bounds), getBottomBound(bounds)); }

	inline sf::Vector2u getTwoDimensionalPositionFromOneDimensionalArrayIndex(unsigned index, unsigned numberOfColumns)
	{ return sf::Vector2u(index % numberOfColumns, index / numberOfColumns); }

	inline bool areTheyOverlapping(sf::FloatRect A, sf::FloatRect B)
	{
		//AABB collision detection algorithm
		return
		A.left < getRightBound(B) &&
		getRightBound(A) > B.left &&
		A.top < getBottomBound(B) &&
		getBottomBound(A) > B.top;
	}

	inline sf::Vector2f lerp(sf::Vector2f source, sf::Vector2f destination, float speed)
	{
		PH_ASSERT(speed >= 0.f, "Speed cannot be less than 0");
		return source + (destination - source) * speed;
	}
}

}
