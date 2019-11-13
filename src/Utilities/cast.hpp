#pragma once

#include "vector4.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <unordered_map>

namespace ph {

namespace Cast {

	/*const std::unordered_map<std::string, ObjectType> objectTypeMap({
		{ "zombie", ObjectType::Zombie },
		{ "npc", ObjectType::Npc }
	});

	ObjectType toObjectType(const std::string& str);*/
	unsigned toUnsigned(const std::string& str);
	bool toBool(const std::string& str);
	std::string toString(const sf::Vector2f&);
	Vector4f toNormalizedColorVector4f(const sf::Color&);
}

}
