#pragma once

#include "vector4.hpp"
#include "forceInline.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <unordered_map>
#include "aliases.hpp"

namespace ph {

FORCE_INLINE u32 castToU32(const std::string& str);
FORCE_INLINE bool castToBool(const std::string& str);
FORCE_INLINE std::string castToString(Vec2);
FORCE_INLINE Vec4 castToNormalizedColorVec4(sf::Color);
FORCE_INLINE sf::Color castToPackedColor(Vec4);

}

#include "cast.inl"
