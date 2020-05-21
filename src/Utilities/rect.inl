#include "rect.hpp"

template<typename T>
FORCE_INLINE Rect<T>::Rect() :x(0), y(0), w(0), h(0) {}

template<typename T>
FORCE_INLINE Rect<T>::Rect(T x, T y, T w, T h) :x(x), y(y), w(w), h(h) {}

template<typename T>
FORCE_INLINE Rect<T>::Rect(sf::Vector2<T> pos, sf::Vector2<T> size) :pos(pos), size(size) {}

template <typename T>
template <typename U>
FORCE_INLINE Rect<T>::Rect(const Rect<U>& rectangle) :
	x(static_cast<T>(rectangle.x)),
	y(static_cast<T>(rectangle.y)),
	w(static_cast<T>(rectangle.w)),
	h(static_cast<T>(rectangle.h)) 
{}

template<typename T>
FORCE_INLINE T Rect<T>::right() const
{
	return x + w;
}

template<typename T>
FORCE_INLINE T Rect<T>::bottom() const
{
	return y + h;
}

template<typename T>
FORCE_INLINE sf::Vector2<T> Rect<T>::center() const
{
	return { x + w / 2, y + h / 2};
}

template<typename T>
FORCE_INLINE sf::Vector2<T> Rect<T>::topRight() const
{
	return { right(), y };
}

template<typename T>
FORCE_INLINE sf::Vector2<T> Rect<T>::bottomLeft() const
{
	return { x, bottom() };
}

template<typename T>
FORCE_INLINE sf::Vector2<T> Rect<T>::bottomRight() const
{
	return { right(), bottom() };
}

template<typename T>
FORCE_INLINE bool Rect<T>::contains(const sf::Vector2<T>& point) const
{
	auto r = right();
	auto b = bottom();

	T minX = std::min(x, r);
	T maxX = std::max(x, r);
	T minY = std::min(y, b);
	T maxY = std::max(y, b);

	return (point.x >= minX) && (point.x <= maxX) && (point.y >= minY) && (point.y <= maxY);
}

template<typename T>
FORCE_INLINE bool intersect(const Rect<T>& a, const Rect<T>& b)
{
	// this function only works properly for rects with positive w and h
	return a.x < b.x + b.w
		&& a.x + a.w > b.x
		&& a.y < b.y + b.h
		&& a.y + a.h > b.y;
}

template<typename T>
FORCE_INLINE bool fullyIntersect(const Rect<T>& a, const Rect<T>& b)
{
	return a.contains(b.pos) && a.contains(b.topRight()) &&
	       a.contains(b.bottomLeft()) && a.contains(b.bottomRight());
}

template<typename T>
FORCE_INLINE bool intersectFlipAllowed(const Rect<T>& a, const Rect<T>& b)
{
    Rect<T> intersection;
    return a.intersects(b, intersection);
}

template<typename T>
FORCE_INLINE bool Rect<T>::intersects(const Rect<T>& rectangle, Rect<T>& intersection) const
{
    // Rectangles with negative (fliped) dimensions are allowed, so we must handle them correctly

    // Compute the min and max of the first rectangle on both axes
    T r1MinX = std::min(x, static_cast<T>(x + w));
    T r1MaxX = std::max(x, static_cast<T>(x + w));
    T r1MinY = std::min(y, static_cast<T>(y + h));
    T r1MaxY = std::max(y, static_cast<T>(y + h));

    // Compute the min and max of the second rectangle on both axes
    T r2MinX = std::min(rectangle.x, static_cast<T>(rectangle.x + rectangle.w));
    T r2MaxX = std::max(rectangle.x, static_cast<T>(rectangle.x + rectangle.w));
    T r2MinY = std::min(rectangle.y, static_cast<T>(rectangle.y + rectangle.h));
    T r2MaxY = std::max(rectangle.y, static_cast<T>(rectangle.y + rectangle.h));

    // Compute the intersection boundaries
    T interLeft   = std::max(r1MinX, r2MinX);
    T interTop    = std::max(r1MinY, r2MinY);
    T interRight  = std::min(r1MaxX, r2MaxX);
    T interBottom = std::min(r1MaxY, r2MaxY);

    // If the intersection is valid (positive non zero area), then there is an intersection
    if ((interLeft < interRight) && (interTop < interBottom))
    {
        intersection = Rect<T>(interLeft, interTop, interRight - interLeft, interBottom - interTop);
        return true;
    }
    else
    {
        intersection = Rect<T>(0, 0, 0, 0);
        return false;
    }
}

template<typename T>
FORCE_INLINE bool Rect<T>::touch(const Rect<T>& rect, sf::Vector2<short>& direction) const
{
	bool collideOnX = x < rect.right() && right() > rect.x;
	bool collideOnY = y < rect.bottom() && bottom() > rect.y;

	if (!(collideOnX ^ collideOnY))
	{
		direction = { 0, 0 };
		return false;
	}

	if (collideOnX)
	{
		if (y == rect.bottom())
		{
			direction = { 0, 1 };
			return true;
		}
		if (bottom() == rect.y)
		{
			direction = { 0, -1 };
			return true;
		}
	}
	if (collideOnY)
	{
		if (x == rect.right())
		{
			direction = { 1, 0 };
			return true;
		}
		if (right() == rect.x)
		{
			direction = { -1, 0 };
			return true;
		}
	}

	direction = { 0, 0 };
	return false;
}

template <typename T>
FORCE_INLINE bool operator ==(const Rect<T>& a, const Rect<T>& b)
{
	return a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h;
}

template <typename T>
FORCE_INLINE bool operator !=(const Rect<T>& a, const Rect<T>& b)
{
	return !(a == b); 
}

