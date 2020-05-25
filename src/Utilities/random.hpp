#pragma once

#include <SFML/System/Vector2.hpp>

namespace ph::Random {

	float generateNumber(float min, float max);
	int generateNumber(int min, int max);
	sf::Vector2f generateVector(sf::Vector2f min, sf::Vector2f max);
	sf::Color generateColor();
	sf::Color generateColor(sf::Color min, sf::Color max);
}
