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

	void setWriteLogsOnlyFromCertainModules(std::vector<ModuleID> moduleID);
	void setWritingLogsFromEachModule();

	void setWritingLogsOnlyFromCertainTypes(std::vector<LogType> logType);
	void setWritingLogsFromEachLogType();

	void setWritingEachLog();

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
