#ifndef POPHEAD_UTILITIES_RANDOM_H_
#define POPHEAD_UTILITIES_RANDOM_H_

#include <random>

namespace PopHead {
namespace Utilities {

class Random {
public:
	static auto generateNumber(float min, float max) -> float;

private:
	static std::default_random_engine mEngine;
};

}
}

#endif
