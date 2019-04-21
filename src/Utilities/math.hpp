#ifndef POPHEAD_UTILITIES_MATH_H_
#define POPHEAD_UTILITIES_MATH_H_

#include <SFML/Graphics.hpp>
#include <cassert>

namespace PopHead {
namespace Utilities {

namespace Math {
	inline float getRightBound(sf::FloatRect bounds) { return bounds.left + bounds.width; }

	inline float getBottomBound(sf::FloatRect bounds) { return bounds.top + bounds.height; }

	inline sf::Vector2f getCenter(sf::FloatRect bounds) { return sf::Vector2f(getRightBound(bounds) / 2.f, getBottomBound(bounds) / 2.f); }

	inline sf::Vector2f lerp(sf::Vector2f source, sf::Vector2f destination, float speed)
	{
		assert(speed >= 0.f, "speed can't be less than 0");
		return source + (destination - source) * speed;
	}
}

}
}

#endif
