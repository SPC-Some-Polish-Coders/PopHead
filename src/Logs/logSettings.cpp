#include "logSettings.hpp"
#include "Logs/LogsInitializer/logsInitializer.hpp"
#include <fstream>

using ph::Logs::LogSettings;
using ph::Logs::LogType;
using ph::Logs::LogData;
using ph::Utilities::IniLoader;

LogSettings::LogSettings()
	: mShouldLogIntoConsole(LogsInitializer::getShouldLogIntoConsole()),
	mShouldLogIntoFile(LogsInitializer::getShouldLogIntoFile())
{
	setLogTypesToWrite(LogsInitializer::getLogTypesToWrite());
	setModuleNamesToWrite(LogsInitializer::getModuleNamesToWrite());
}

void LogSettings::turnOnWritingEachLog()
{
	turnOnWritingLogsFromEachModule();
	turnOnWritingLogsFromEachLogTypes();
}

void LogSettings::turnOnWritingLogsFromEachModule()
{
	setModuleNamesToWrite(
		{ "Audio", "Base", "Input", "Logs", "Physics", "Renderer", "Resources", "States", "Utilities", "World", "Terminal", "None" }
	);
}

void LogSettings::turnOnWritingLogsFromEachLogTypes() 
{
	const std::size_t count = static_cast<std::size_t>(LogType::Count);
	mLogTypesToWrite.clear();
	mLogTypesToWrite.resize(count);
	for (std::size_t i = 0; i < count; ++i)
		mLogTypesToWrite[i] = static_cast<LogType>(i);
}

void LogSettings::setWritingLogs(bool enabled)
{
	mShouldLogIntoConsole = enabled;
	mShouldLogIntoFile = enabled;
}

bool LogSettings::shouldBeWrittenIntoConsole(const LogData& log) const
{
	return mShouldLogIntoConsole && shouldBeWritten(log);
}

bool LogSettings::shouldBeWrittenIntoFile(const LogData& log) const
{
	return mShouldLogIntoFile && shouldBeWritten(log);
}

bool LogSettings::shouldBeWritten(const LogData& log) const
{
	return shouldBeWrittenConsideringLogType(log) && shouldBeWrittenConsideringModuleName(log);
}

bool LogSettings::shouldBeWrittenConsideringLogType(const LogData& log) const
{
	for (LogType type : mLogTypesToWrite) 
		if (type == log.type) 
			return true;
	return false;
}

bool LogSettings::shouldBeWrittenConsideringModuleName(const LogData& log) const
{
	for (const std::string& moduleName : mModuleNamesToWrite)
		if (moduleName == log.moduleName)
			return true;
	return false;
}

