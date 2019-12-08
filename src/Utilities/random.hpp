#pragma once

#include <random>
#include <ctime>
#include <SFML/System/Vector2.hpp>

namespace ph {

class Random {
public:
	static float generateNumber(const float min, const float max);
	static int generateNumber(const int min, const int max);
	static sf::Vector2f generateVector(const sf::Vector2f min, const sf::Vector2f max);

private:
	inline static std::default_random_engine mEngine = std::default_random_engine(static_cast<unsigned>(time(nullptr)));
};

}
