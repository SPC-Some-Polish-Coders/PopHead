#include "rect.hpp"

template<typename T>
T ph::Rect<T>::right() const
{
	return left + width;
}

template<typename T>
inline T ph::Rect<T>::bottom() const
{
	return top + height;
}

template<typename T>
inline sf::Vector2<T> ph::Rect<T>::getCenter() const
{
	return { left + width / 2, top + height / 2};
}

template<typename T>
inline sf::Vector2<T> ph::Rect<T>::getTopLeft() const
{
	return { left, top };
}

template<typename T>
inline sf::Vector2<T> ph::Rect<T>::getTopRight() const
{
	return { right(), top };
}

template<typename T>
inline sf::Vector2<T> ph::Rect<T>::getBottomLeft() const
{
	return { left, bottom() };
}

template<typename T>
inline sf::Vector2<T> ph::Rect<T>::getBottomRight() const
{
	return { right(), bottom() };
}

template<typename T>
inline bool ph::Rect<T>::containsIncludingBounds(const sf::Vector2<T>& point) const
{
	auto r = right();
	auto b = bottom();

	T minX = std::min(left, r);
	T maxX = std::max(left, r);
	T minY = std::min(top, b);
	T maxY = std::max(top, b);

	return (x >= minX) && (x <= maxX) && (y >= minY) && (y <= maxY);
}
