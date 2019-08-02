#include "random.hpp"
#include "Logs/logs.hpp"

namespace ph {

float Random::generateNumber(const float min, const float max)
{
	PH_ASSERT(min <= max, "Min can't be greater than max");
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mEngine);
}

int Random::generateNumber(const int min, const int max)
{
	PH_ASSERT(min <= max, "Min can't be greater than max");
	std::uniform_int_distribution<> dist(min, max);
	return dist(mEngine);
}

}