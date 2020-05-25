#include "pch.hpp"
#include "random.hpp"

namespace ph::Random {

static std::default_random_engine engine = std::default_random_engine(static_cast<unsigned>(time(nullptr)));

float generateNumber(float min, float max)
{
	PH_ASSERT_UNEXPECTED_SITUATION(min <= max, "Min can't be greater than max");
	std::uniform_real_distribution<float> dist(min, max);
	return dist(engine);
}

int generateNumber(int min, int max)
{
	PH_ASSERT_UNEXPECTED_SITUATION(min <= max, "Min can't be greater than max");
	std::uniform_int_distribution<> dist(min, max);
	return dist(engine);
}

sf::Vector2f generateVector(sf::Vector2f min, sf::Vector2f max)
{
	PH_ASSERT_UNEXPECTED_SITUATION(min.x <= max.x && min.y <= max.y, "Min can't be greater than max");
	std::uniform_real_distribution<float> distX(min.x, max.x);
	std::uniform_real_distribution<float> distY(min.y, max.y);
	return sf::Vector2f(distX(engine), distY(engine));
}

sf::Color generateColor()
{
	std::uniform_int_distribution<> dist(0, 255);
	return sf::Color(dist(engine), dist(engine), dist(engine), dist(engine));
}

sf::Color generateColor(sf::Color min, sf::Color max)
{
	std::uniform_int_distribution<> distR(min.r, max.r);
	std::uniform_int_distribution<> distG(min.g, max.g);
	std::uniform_int_distribution<> distB(min.b, max.b);
	std::uniform_int_distribution<> distA(min.a, max.a);
	return sf::Color(distR(engine), distG(engine), distB(engine), distA(engine));
}

}
