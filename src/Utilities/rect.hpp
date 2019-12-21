#pragma once

#include <SFML/Graphics/Rect.hpp>
#include "Utilities/forceInline.hpp"

namespace ph {

template<typename T>
class Rect : public sf::Rect<T>
{
public:
	// use all constructors from sf::Rect
	using sf::Rect<T>::Rect;

	Rect(const sf::Rect<T>& rect);
	Rect& operator=(const sf::Rect<T>& rect);

	FORCE_INLINE void setPosition(sf::Vector2<T>);
	FORCE_INLINE void setSize(sf::Vector2<T>);

	FORCE_INLINE T right() const;
	FORCE_INLINE T bottom() const;

	FORCE_INLINE sf::Vector2<T> getCenter() const;
	FORCE_INLINE sf::Vector2<T> getTopLeft() const;
	FORCE_INLINE sf::Vector2<T> getTopRight() const;
	FORCE_INLINE sf::Vector2<T> getBottomLeft() const;
	FORCE_INLINE sf::Vector2<T> getBottomRight() const;
	
	FORCE_INLINE sf::Vector2<T> getSize() const;

	FORCE_INLINE bool containsIncludingBounds(const sf::Vector2<T>& point) const;

	FORCE_INLINE bool doPositiveRectsIntersect(const sf::Rect<T>& rect) const;
	FORCE_INLINE static bool doPositiveRectsIntersect(const sf::Rect<T>& a, const sf::Rect<T>& b);
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

}

#include "rect.inl"
