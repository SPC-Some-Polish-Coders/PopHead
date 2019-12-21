#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace ph {

template<typename T>
class Rect : public sf::Rect<T>
{
public:

	// use all constructors from sf::Rect
	using sf::Rect<T>::Rect;

	Rect(const sf::Rect<T>& rect);
	Rect& operator=(const sf::Rect<T>& rect);

	void setPosition(sf::Vector2<T>);

	T right() const;
	T bottom() const;

	sf::Vector2<T> getCenter() const;
	sf::Vector2<T> getTopLeft() const;
	sf::Vector2<T> getTopRight() const;
	sf::Vector2<T> getBottomLeft() const;
	sf::Vector2<T> getBottomRight() const;
	
	sf::Vector2<T> getSize() const;

	bool containsIncludingBounds(const sf::Vector2<T>& point) const;

	bool doPositiveRectsIntersect(const sf::Rect<T>& rect) const;
	static bool doPositiveRectsIntersect(const sf::Rect<T>& a, const sf::Rect<T>& b);
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

}

#include "rect.inl"
