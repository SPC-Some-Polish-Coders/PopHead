#include "random.hpp"
#include "Logs/logs.hpp"
#include <random>
#include <ctime>

namespace ph::Random {

static std::default_random_engine engine = std::default_random_engine(static_cast<unsigned>(time(nullptr)));

float generateNumber(const float min, const float max)
{
	PH_ASSERT_UNEXPECTED_SITUATION(min <= max, "Min can't be greater than max");
	std::uniform_real_distribution<float> dist(min, max);
	return dist(engine);
}

int generateNumber(const int min, const int max)
{
	PH_ASSERT_UNEXPECTED_SITUATION(min <= max, "Min can't be greater than max");
	std::uniform_int_distribution<> dist(min, max);
	return dist(engine);
}

sf::Vector2f generateVector(const sf::Vector2f min, const sf::Vector2f max)
{
	PH_ASSERT_UNEXPECTED_SITUATION(min.x <= max.x && min.y <= max.y, "Min can't be greater than max");
	std::uniform_real_distribution<float> distX(min.x, max.x);
	std::uniform_real_distribution<float> distY(min.y, max.y);
	return sf::Vector2f(distX(engine), distY(engine));
}

}
