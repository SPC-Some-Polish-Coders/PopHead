#pragma once

#include <SFML/System/Vector2.hpp>

namespace ph::Random {

	float generateNumber(float min, float max);
	i32 generateNumber(i32 min, i32 max);
	size_t generateNumber(size_t min, size_t max);
	Vec2 generateVector(Vec2 min, Vec2 max);
	sf::Color generateColor();
	sf::Color generateColor(sf::Color min, sf::Color max);
}
