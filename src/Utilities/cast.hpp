#ifndef POPHEAD_UTILITIES_CAST_H_
#define POPHEAD_UTILITIES_CAST_H_

#include "SFML/System/Vector2.hpp"
#include <string>

namespace ph {

namespace Cast {
	unsigned toUnsigned(const std::string& str);
	std::string toString(const sf::Vector2f&);
}

}

#endif // !POPHEAD_UTILITIES_CAST_H_
