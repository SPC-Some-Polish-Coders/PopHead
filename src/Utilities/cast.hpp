#pragma once

#include "vector4.hpp"
#include "forceInline.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <unordered_map>
#include "aliases.hpp"

namespace ph {

FORCE_INLINE u32 toU32(const std::string& str);
FORCE_INLINE bool toBool(const std::string& str);
FORCE_INLINE std::string toString(Vec2);
FORCE_INLINE Vec4 toNormalizedColorVec4(sf::Color);
FORCE_INLINE sf::Color toPackedColor(Vec4);

}

#include "cast.inl"
