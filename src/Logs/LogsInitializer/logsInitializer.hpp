#ifndef POPHEAD_LOGS_LOGSINITIALIZER_H_
#define	POPHEAD_LOGS_LOGSINITIALIZER_H_

#include "Utilities/iniLoader.hpp"
#include "Logs/log.hpp"

namespace ph {
namespace Logs {

class LogsInitializer : private Utilities::IniLoader
{
public:
	static bool getShouldLogIntoConsole();
	static bool getShouldLogIntoFile();
	static std::vector<Logs::LogType> getLogTypesToWrite();
	static std::vector<std::string> getModuleNamesToWrite();
};

}}

#endif // !POPHEAD_LOGS_LOGSINITIALIZER_H_