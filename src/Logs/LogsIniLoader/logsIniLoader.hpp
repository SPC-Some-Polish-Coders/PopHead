#ifndef POPHEAD_LOGS_LOGSINILOADER_H_
#define	POPHEAD_LOGS_LOGSINILOADER_H_

#include "Utilities/iniLoader.hpp"

namespace PopHead {
namespace Logs {

class LogsIniLoader : private Utilities::IniLoader
{
public:
	static bool getShouldLogIntoConsole();
	static bool getShouldLogIntoFile();
	static std::vector<Logs::LogType> getLogTypesToWrite();
	static std::vector<std::string> getModuleNamesToWrite();
};

}}

#endif // !POPHEAD_LOGS_LOGSINILOADER_H_