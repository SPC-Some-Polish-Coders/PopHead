#include "random.hpp"
#include "Logs/assert.hpp"
#include <ctime>

std::default_random_engine PopHead::Utilities::Random::mEngine(static_cast<unsigned>(time(nullptr)));

float PopHead::Utilities::Random::generateNumber(float min, float max)
{
	PH_ASSERT(min <= max, "Min can't be greater than max");
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mEngine);
}
