#pragma once

#include <random>
#include <ctime>

namespace ph {

class Random {
public:
	static float generateNumber(const float min, const float max);
	static int generateNumber(const int min, const int max);

private:
	inline static std::default_random_engine mEngine = std::default_random_engine(static_cast<unsigned>(time(nullptr)));
};

}
