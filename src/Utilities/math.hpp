#ifndef POPHEAD_UTILITIES_MATH_H_
#define POPHEAD_UTILITIES_MATH_H_

#include "Utilities/debug.hpp"
#include <SFML/Graphics.hpp>

namespace PopHead {
namespace Utilities {

namespace Math {
	inline float getRightBound(sf::FloatRect bounds) 
	{ return bounds.left + bounds.width; }

	inline float getBottomBound(sf::FloatRect bounds)  
	{ return bounds.top + bounds.height; }

	inline sf::Vector2f getCenter(sf::FloatRect bounds)
	{ return sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f); }

	inline sf::Vector2f lerp(sf::Vector2f source, sf::Vector2f destination, float speed)
	{
		PH_ASSERT(speed >= 0.f, "Speed cannot be less than 0");
		return source + (destination - source) * speed;
	}
}

}}

#endif
