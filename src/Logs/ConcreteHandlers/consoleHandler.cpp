#include "consoleHandler.hpp"

#include <iostream>
#include <iomanip>

namespace ph {

	void ConsoleHandler::utilizeLog(const LogRecord& logRecord)
	{
		std::cout << "[  " << std::left << std::setw(7)
				  << std::to_string(logRecord.secondsFromStart).erase(5, 4) << "s ]"
				  << " | " << std::setw(9) << std::left << logLevelToString(logRecord.level)
				  << " | " << std::setw(9) << std::left << logRecord.moduleName
				  << " | " << std::left << logRecord.message << std::endl;
	}
}
