#pragma once

#include "system.hpp"

#include <vector>
#include <memory>

namespace ph {

	class SystemsQueue
	{
	public:

	void appendSystem(std::unique_ptr<System>&& system);
		void update(float seconds);

	private:
		std::vector<std::unique_ptr<System>> mSystemsArray;
	};
}
