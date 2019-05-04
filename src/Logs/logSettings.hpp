#ifndef POPHEAD_LOGS_LOGSETTINGS_H_
#define POPHEAD_LOGS_LOGSETTINGS_H_

#include "LogData/Log.hpp"
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
	void setWritingLogsFromEachModules();

	void setWritingLogsOnlyFromCertainTypes(const std::vector<LogType>& logTypesToWrite) { mTypesOfLogToWrite = logTypesToWrite;}
	void setWritingLogsFromEachLogTypes();

	bool shouldThisLogBeWrittenIntoConsole(const Log&) const;
	bool shouldThisLogBeWrittenIntoFile(const Log&) const;
private:
	bool shouldThisLogBeWritten(const Log&) const;
	bool shouldThisLogBeWrittenConsideringLogType(const Log&) const;
	bool shouldThisLogBeWrittenConsideringModuleID(const Log&) const;

private:
	std::vector<ModuleID> mLogFromModulesToWrite;
	std::vector<LogType> mTypesOfLogToWrite;
	bool mShouldLogIntoConsole;
	bool mShouldLogIntoFile;
};

}}

#endif //POPHEAD_LOGS_LOGSETTINGS_H_
