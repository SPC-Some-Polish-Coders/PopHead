#include "criticalError.hpp"

namespace ph {

	CriticalError::CriticalError(const std::string& message)
		: std::runtime_error(message)
	{
	}
}
