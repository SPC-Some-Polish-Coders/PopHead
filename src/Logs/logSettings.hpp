#pragma once

#include "log.hpp"
#include <vector>

namespace ph{

class LogSettings
{
public:
	LogSettings();

public:
	void setWritingLogs(bool enabled);
	void setWritingLogsIntoConsole(bool enabled) { mShouldLogIntoConsole = enabled; }
	void setWritingLogsIntoFile(bool enabled) { mShouldLogIntoFile = enabled; }
	void setWritingLogsIntoTerminal(bool enabled) { mShouldLogIntoTerminal = enabled; }
	void addToVector(const LogType&);
	void addToVector(const std::string&);

	void turnOnWritingEachLog();

	void setModuleNamesToWrite(const std::vector<std::string>& moduleNames) {mModuleNamesToWrite = moduleNames;}
	void turnOnWritingLogsFromEachModule();

	void setLogTypesToWrite(const std::vector<LogType>& logTypes) { mLogTypesToWrite = logTypes;}
	void turnOnWritingLogsFromEachLogTypes();

	bool shouldBeWrittenIntoConsole(const LogData&) const;
	bool shouldBeWrittenIntoFile(const LogData&) const;
	bool shouldBeWrittenIntoTerminal(const LogData&) const;

private:
	bool shouldBeWritten(const LogData&) const;
	bool shouldBeWrittenConsideringLogType(const LogData&) const;
	bool shouldBeWrittenConsideringModuleName(const LogData&) const;

private:
	std::vector<std::string> mModuleNamesToWrite;
	std::vector<LogType> mLogTypesToWrite;
	bool mShouldLogIntoConsole;
	bool mShouldLogIntoFile;
	bool mShouldLogIntoTerminal;
};

}
