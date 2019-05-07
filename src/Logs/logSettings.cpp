#include "logSettings.hpp"

using PopHead::Logs::LogSettings;
using PopHead::Logs::LogType;
using PopHead::Logs::Log;

LogSettings::LogSettings()
	:mShouldLogIntoConsole(true)
	,mShouldLogIntoFile(true)
{
	turnOnWritingEachLog();
}

void LogSettings::turnOnWritingEachLog()
{
	turnOnWritingLogsFromEachModule();
	turnOnWritingLogsFromEachLogTypes();
}

void LogSettings::turnOnWritingLogsFromEachModule()
{
	setModuleNamesToWrite(
		{ "Audio", "Base", "Input", "Logs", "Physics", "Renderer", "Resources", "States", "Utilities", "World", "None" }
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

bool LogSettings::shouldBeWrittenIntoConsole(const Log& log) const
{
	return mShouldLogIntoConsole && shouldBeWritten(log);
}

bool LogSettings::shouldBeWrittenIntoFile(const Log& log) const
{
	return mShouldLogIntoFile && shouldBeWritten(log);
}

bool LogSettings::shouldBeWritten(const Log& log) const
{
	return shouldBeWrittenConsideringLogType(log) && shouldBeWrittenConsideringModuleName(log);
}

bool LogSettings::shouldBeWrittenConsideringLogType(const Log& log) const
{
	for (auto type : mLogTypesToWrite) 
		if (type == log.type) 
			return true;
	return false;
}

bool LogSettings::shouldBeWrittenConsideringModuleName(const Log& log) const
{
	for (auto moduleName : mModuleNamesToWrite)
		if (moduleName == log.moduleName)
			return true;
	return false;
}