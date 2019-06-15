#ifndef POPHEAD_LOGS_LOGSINITIALIZER_H_
#define	POPHEAD_LOGS_LOGSINITIALIZER_H_

#include "Utilities/iniLoader.hpp"
#include "Logs/log.hpp"

namespace ph {

class LogsInitializer : private IniLoader
{
public:
	static bool getShouldLogIntoConsole();
	static bool getShouldLogIntoFile();
	static std::vector<LogType> getLogTypesToWrite();
	static std::vector<std::string> getModuleNamesToWrite();
};

}

#endif // !POPHEAD_LOGS_LOGSINITIALIZER_H_