#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Utilities/forceInline.hpp"
#include <algorithm>
#include "aliases.hpp"

namespace ph {

template<typename T>
struct Rect
{
	FORCE_INLINE Rect();
	FORCE_INLINE Rect(T x, T y, T w, T h);
	FORCE_INLINE Rect(Vec2Base<T> pos, Vec2Base<T> size);

    template <typename U>
    FORCE_INLINE explicit Rect(const Rect<U>& rectangle);

	FORCE_INLINE void setCenter(Vec2Base<T>);

	FORCE_INLINE T right() const;
	FORCE_INLINE T bottom() const;

	FORCE_INLINE Vec2Base<T> center() const;
	FORCE_INLINE Vec2Base<T> topRight() const;
	FORCE_INLINE Vec2Base<T> bottomLeft() const;
	FORCE_INLINE Vec2Base<T> bottomRight() const;
	FORCE_INLINE Vec2Base<T> topCenter() const;
	FORCE_INLINE Vec2Base<T> bottomCenter() const;
	FORCE_INLINE Vec2Base<T> leftCenter() const;
	FORCE_INLINE Vec2Base<T> rightCenter() const;

	FORCE_INLINE bool contains(const Vec2Base<T>& point) const;
    FORCE_INLINE bool intersects(const Rect<T>& rectangle, Rect<T>& intersection) const;
	FORCE_INLINE bool touch(const Rect<T>& rect, Vec2Base<i16>& direction) const;

	union
	{
		struct { T x, y, w, h; };
		struct { Vec2Base<T> pos, size; };
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

using IntRect = Rect<i32>;
using FloatRect = Rect<float>;

}

