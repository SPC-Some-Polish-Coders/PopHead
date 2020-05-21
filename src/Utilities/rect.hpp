#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Utilities/forceInline.hpp"
#include <algorithm>

namespace ph {

template<typename T>
struct Rect
{
	FORCE_INLINE Rect();
	FORCE_INLINE Rect(T x, T y, T w, T h);
	FORCE_INLINE Rect(sf::Vector2<T> pos, sf::Vector2<T> size);

    template <typename U>
    FORCE_INLINE explicit Rect(const Rect<U>& rectangle);

	FORCE_INLINE T right() const;
	FORCE_INLINE T bottom() const;

	FORCE_INLINE sf::Vector2<T> center() const;
	FORCE_INLINE sf::Vector2<T> topRight() const;
	FORCE_INLINE sf::Vector2<T> bottomLeft() const;
	FORCE_INLINE sf::Vector2<T> bottomRight() const;

	FORCE_INLINE bool contains(const sf::Vector2<T>& point) const;
    FORCE_INLINE bool intersects(const Rect<T>& rectangle, Rect<T>& intersection) const;
	FORCE_INLINE bool touch(const Rect<T>& rect, sf::Vector2<short>& direction) const;

	union
	{
		struct { T x, y, w, h; };
		struct { sf::Vector2<T> pos, size; };
	};
};

template<typename T>
FORCE_INLINE bool intersect(const Rect<T>& a, const Rect<T>& b);

template<typename T>
FORCE_INLINE bool fullyIntersect(const Rect<T>& a, const Rect<T>& b);

template<typename T>
FORCE_INLINE bool intersectFlipAllowed(const Rect<T>& a, const Rect<T>& b);

template <typename T>
FORCE_INLINE bool operator ==(const Rect<T>& a, const Rect<T>& b);

template <typename T>
FORCE_INLINE bool operator !=(const Rect<T>& a, const Rect<T>& b);

#include "rect.inl"

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

}

