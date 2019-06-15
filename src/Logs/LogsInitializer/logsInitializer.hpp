#ifndef POPHEAD_LOGS_LOGSINITIALIZER_H_
#define	POPHEAD_LOGS_LOGSINITIALIZER_H_

#include "Utilities/iniLoader.hpp"
#include "Logs/log.hpp"

namespace ph {

class LogsInitializer : private IniLoader
{
public:
	static std::vector<LogType> getLogTypesToWrite();
	static std::vector<std::string> getModuleNamesToWrite();

	static bool getShouldLogIntoConsole();
	static bool getShouldLogIntoFile();
private:
	enum class LogOutputTarget{ console, file };
	static bool getShouldLogInto(LogOutputTarget);
};

}

#endif // !POPHEAD_LOGS_LOGSINITIALIZER_H_