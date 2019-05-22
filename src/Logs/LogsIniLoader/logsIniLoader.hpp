#ifndef POPHEAD_LOGS_LOGSINILOADER_H_
#define	POPHEAD_LOGS_LOGSINILOADER_H_

#include "Utilities/iniLoader.hpp"
#include "Logs/log.hpp"

namespace PopHead {
namespace Logs {

class LogsIniLoader : private Utilities::IniLoader
{
public:
	static bool iniGetShouldLogIntoConsole();
	static bool iniGetShouldLogIntoFile();
	static std::vector<Logs::LogType> iniGetLogTypesToWrite();
	static std::vector<std::string> iniGetModuleNamesToWrite();
};

}}

#endif // !POPHEAD_LOGS_LOGSINILOADER_H_