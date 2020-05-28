#include <cstdint>
#include <SFML/System/Vector2.hpp>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

using i64 = int64_t;
using i32 = int32_t;
using i16 = int16_t;
using i8 = int8_t;

template<typename T>
using Vec2Base = sf::Vector2<T>;

using Vec2 = sf::Vector2f;
using Vec2i = sf::Vector2i;
using Vec2u = sf::Vector2u;

#define Cast static_cast
#define Null nullptr
