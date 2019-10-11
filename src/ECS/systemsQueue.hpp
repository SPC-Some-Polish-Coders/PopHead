#pragma once

#include "system.hpp"

#include <vector>
#include <memory>

namespace ph {

class SystemsQueue
{
public:
	void update(float seconds);

	void appendSystem(std::unique_ptr<System>&& system);

private:
	std::vector<std::unique_ptr<System>> mSystemsArray;
};
}
