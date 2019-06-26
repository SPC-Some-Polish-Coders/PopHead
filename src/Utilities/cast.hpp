#pragma once

#include "SFML/System/Vector2.hpp"
#include <string>

namespace ph {

namespace Cast {
	unsigned toUnsigned(const std::string& str);
	std::string toString(const sf::Vector2f&);
	bool toBool(const std::string& str);
}

}
