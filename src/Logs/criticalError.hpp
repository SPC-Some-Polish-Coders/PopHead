#pragma once

#include <stdexcept>

namespace ph {

	class CriticalError : public std::runtime_error
	{
	public:
		explicit CriticalError(const std::string& message);
	};
}