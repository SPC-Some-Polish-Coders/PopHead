#include "pch.hpp"
#include "WindowsDebugApi.hpp"

#ifdef PH_WINDOWS
#include <Windows.h>
#include <debugapi.h>
#endif // PH_WINDOWS

namespace ph {

	void WindowsDebugApiHandler::utilizeLog(const LogRecord& logRecord)
	{
		#ifdef PH_WINDOWS

		std::ostringstream stream;
		
		stream << "[  " << std::left << std::setw(7)
			<< std::to_string(logRecord.secondsFromStart).erase(5, 4) << "s ]"
			<< " | " << std::setw(9) << std::left << logLevelToString(logRecord.level)
			<< " | " << std::setw(9) << std::left << logRecord.filePath
			<< " | " << std::left << logRecord.message << std::endl;

		OutputDebugStringA(stream.str().c_str());
		
		
		#endif // PH_WINDOWS
	}
}
