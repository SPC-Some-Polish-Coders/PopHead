#pragma once

#include <SFML/Graphics.hpp>

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

	sf::Vector2u getTwoDimensionalPositionFromOneDimensionalArrayIndex(const unsigned index, const unsigned numberOfColumns);

	bool areTheyOverlapping(const sf::FloatRect& A, const sf::FloatRect& B);

	sf::Vector2f lerp(const sf::Vector2f source, const sf::Vector2f destination, const float speed);
}

}
