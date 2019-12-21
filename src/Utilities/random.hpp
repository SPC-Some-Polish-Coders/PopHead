#pragma once

#include <SFML/System/Vector2.hpp>

namespace ph::Random {

	float generateNumber(const float min, const float max);
	int generateNumber(const int min, const int max);
	sf::Vector2f generateVector(const sf::Vector2f min, const sf::Vector2f max);
}
