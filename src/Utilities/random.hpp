#pragma once

#include <random>

namespace ph {

class Random {
public:
	static float generateNumber(float min, float max);

private:
	static std::default_random_engine mEngine;
};

}
