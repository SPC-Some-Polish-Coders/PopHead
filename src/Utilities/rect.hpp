#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace ph {

template<typename T>
class Rect : public sf::Rect<T>
{
public:

	// use all constructors from sf::Rect
	using sf::Rect<T>::Rect;

	Rect& operator=(const sf::Rect<T>& rect);
	Rect& operator=(sf::Rect<T>&& rect);

	T right() const;
	T bottom() const;

	sf::Vector2<T> getCenter() const;
	sf::Vector2<T> getTopLeft() const;
	sf::Vector2<T> getTopRight() const;
	sf::Vector2<T> getBottomLeft() const;
	sf::Vector2<T> getBottomRight() const;

	bool containsIncludingBounds(const sf::Vector2<T>& point) const;
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

}

#include "rect.inl"
