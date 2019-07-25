#pragma once

#include "Utilities/spawn.hpp"
#include "SFML/System/Vector2.hpp"
#include <string>

namespace ph {

namespace Cast {
	ObjectType toObjectType(const std::string& str);
	unsigned toUnsigned(const std::string& str);
	std::string toString(const sf::Vector2f&);
	bool toBool(const std::string& str);
}

}
