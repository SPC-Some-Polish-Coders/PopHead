#include "math.hpp"
#include "Utilities/debug.hpp"

namespace ph {

namespace Math 
{
	float getRightBound(const sf::FloatRect& bounds)
	{
		return bounds.left + bounds.width;
	}

	float getBottomBound(const sf::FloatRect& bounds)
	{
		return bounds.top + bounds.height;
	}

	sf::Vector2f getCenter(const sf::FloatRect& bounds)
	{
		return sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	}

	sf::Vector2f getTopLeftCorner(const sf::FloatRect& bounds)
	{
		return sf::Vector2f(bounds.left, bounds.top);
	}

	sf::Vector2f getTopRightCorner(const sf::FloatRect& bounds)
	{
		return sf::Vector2f(getRightBound(bounds), bounds.top);
	}

	sf::Vector2f getBottomLeftCorner(const sf::FloatRect& bounds)
	{
		return sf::Vector2f(bounds.left, getBottomBound(bounds));
	}

	sf::Vector2f getBottomRightCorner(const sf::FloatRect& bounds)
	{
		return sf::Vector2f(getRightBound(bounds), getBottomBound(bounds));
	}

	sf::Vector2u getTwoDimensionalPositionFromOneDimensionalArrayIndex(const unsigned index, const unsigned numberOfColumns)
	{
		PH_ASSERT_EXCEPTION(numberOfColumns != 0, "Number of columns cannot be 0");
		return sf::Vector2u(index % numberOfColumns, index / numberOfColumns);
	}

	bool areTheyOverlapping(const sf::FloatRect& A, const sf::FloatRect& B)
	{
		//AABB collision detection algorithm
		return
			A.left < getRightBound(B) &&
			getRightBound(A) > B.left &&
			A.top < getBottomBound(B) &&
			getBottomBound(A) > B.top;
	}

	sf::Vector2f lerp(const sf::Vector2f source, const sf::Vector2f destination, const float speed)
	{
		PH_ASSERT(speed >= 0.f, "Speed cannot be less than 0");
		return source + (destination - source) * speed;
	}
}

}