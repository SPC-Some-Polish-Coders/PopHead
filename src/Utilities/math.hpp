#ifndef POPHEAD_UTILITIES_MATH_H_
#define POPHEAD_UTILITIES_MATH_H_

#include "Logs/assert.hpp"
#include <SFML/Graphics.hpp>

namespace PopHead {
namespace Utilities {

namespace Math {
	inline auto getRightBound(sf::FloatRect bounds) -> float { return bounds.left + bounds.width; }

	inline auto getBottomBound(sf::FloatRect bounds) -> float { return bounds.top + bounds.height; }

	inline auto getCenter(sf::FloatRect bounds) -> sf::Vector2f
	{ return sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f); }

	inline auto lerp(sf::Vector2f source, sf::Vector2f destination, float speed) -> sf::Vector2f
	{
		PH_ASSERT(speed >= 0.f, "Speed cannot be less than 0");
		return source + (destination - source) * speed;
	}
}

}}

#endif
