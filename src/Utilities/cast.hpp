#pragma once

#include "vector4.hpp"
#include "forceInline.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <unordered_map>

namespace ph::Cast {

	FORCE_INLINE unsigned toUnsigned(const std::string& str);
	FORCE_INLINE bool toBool(const std::string& str);
	FORCE_INLINE std::string toString(const sf::Vector2f&);
	FORCE_INLINE Vector4f toNormalizedColorVector4f(const sf::Color&);

}

#include "cast.inl"
