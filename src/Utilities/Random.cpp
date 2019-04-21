#include "Random.h"
#include <ctime>
#include <cassert>

std::default_random_engine PopHead::Utilities::Random::mEngine(static_cast<unsigned>(time(nullptr)));

float PopHead::Utilities::Random::generateNumber(float min, float max)
{
	assert(min <= max, "min can't be greater than max");
	const std::uniform_real_distribution<float> dist(min, max);
	return dist(mEngine);
}
