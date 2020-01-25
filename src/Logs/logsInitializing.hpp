#pragma once

#include <string>

namespace ph {

	class Terminal;

	void initializeLogsModule(const std::string& configFileName, Terminal* terminal);
}
