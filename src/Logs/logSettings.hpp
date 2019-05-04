#ifndef POPHEAD_LOGS_LOGSETTINGS_H_
#define POPHEAD_LOGS_LOGSETTINGS_H_

#include "LogData/moduleID.hpp"
#include "LogData/logType.hpp"
#include <vector>

namespace PopHead{
namespace Logs{


class LogSettings
{
public:
	LogSettings();

public:
	void setWritingLogs(bool enabled);
	void setWritingLogsIntoConsole(bool enabled) { mShouldLogIntoConsole = enabled; }
	void setWritingLogsIntoFile(bool enabled) { mShouldLogIntoFile = enabled; }

	void setWritingEachLog();

	void setWritingLogsOnlyFromCertainModules(const std::vector<ModuleID>& modulesToWrite) {mLogFromModulesToWrite = modulesToWrite;}
	void setWritingLogsFromEachModule();

	void setWritingLogsOnlyFromCertainTypes(const std::vector<LogType>& logTypesToWrite) { mTypesOfLogToWrite = logTypesToWrite;}
	void setWritingLogsFromEachLogType();

private:
	bool shouldThisLogBeWrittenIntoConsole();
	bool shouldThisLogBeWrittenIntoFile();

private:
	std::vector<ModuleID> mLogFromModulesToWrite;
	std::vector<LogType> mTypesOfLogToWrite;
	bool mShouldLogIntoConsole;
	bool mShouldLogIntoFile;

public:
//	friend LogManager;
};

}}

#endif //POPHEAD_LOGS_LOGSETTINGS_H_
