#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace ph {

template<typename T>
class Rect : public sf::Rect<T>
{
public:

	using sf::Rect<T>::Rect;

	T right() const;
	T bottom() const;

	sf::Vector2<T> getCenter() const;
	sf::Vector2<T> getTopLeft() const;
	sf::Vector2<T> getTopRight() const;
	sf::Vector2<T> getBottomLeft() const;
	sf::Vector2<T> getBottomRight() const;

	bool containsIncludingBounds(const sf::Vector2<T>& point) const;
};

}

#include "rect.inl"
