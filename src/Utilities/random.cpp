#include "random.hpp"
#include "Logs/logs.hpp"
#include <ctime>

namespace ph {

std::default_random_engine Random::mEngine(static_cast<unsigned>(time(nullptr)));

float Random::generateNumber(float min, float max)
{
	PH_ASSERT(min <= max, "Min can't be greater than max");
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mEngine);
}

}