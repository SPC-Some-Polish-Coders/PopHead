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

	void turnOnWritingEachLog();

	void setModuleNamesToWrite(const std::vector<std::string>& moduleNames) {mModuleNamesToWrite = moduleNames;}
	void turnOnWritingLogsFromEachModule();

	void setLogTypesToWrite(const std::vector<LogType>& logTypes) { mLogTypesToWrite = logTypes;}
	void turnOnWritingLogsFromEachLogTypes();

	bool shouldBeWrittenIntoConsole(const Log&) const;
	bool shouldBeWrittenIntoFile(const Log&) const;

private:
	bool shouldBeWritten(const Log&) const;
	bool shouldBeWrittenConsideringLogType(const Log&) const;
	bool shouldBeWrittenConsideringModuleName(const Log&) const;

private:
	std::vector<std::string> mModuleNamesToWrite;
	std::vector<LogType> mLogTypesToWrite;
	bool mShouldLogIntoConsole;
	bool mShouldLogIntoFile;
};

}}

#endif //POPHEAD_LOGS_LOGSETTINGS_H_
