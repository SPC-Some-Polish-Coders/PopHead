#include "random.hpp"
#include "Utilities/debug.hpp"
#include <ctime>

std::default_random_engine ph::Random::mEngine(static_cast<unsigned>(time(nullptr)));

float ph::Random::generateNumber(float min, float max)
{
	PH_ASSERT(min <= max, "Min can't be greater than max");
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mEngine);
}
