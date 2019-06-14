#include "logSettings.hpp"
#include "Logs/LogsInitializer/logsInitializer.hpp"
#include <fstream>

ph::LogSettings::LogSettings()
	:mShouldLogIntoConsole(LogsInitializer::getShouldLogIntoConsole())
	,mShouldLogIntoFile(LogsInitializer::getShouldLogIntoFile())
{
	setLogTypesToWrite(LogsInitializer::getLogTypesToWrite());
	setModuleNamesToWrite(LogsInitializer::getModuleNamesToWrite());
}

void ph::LogSettings::turnOnWritingEachLog()
{
	turnOnWritingLogsFromEachModule();
	turnOnWritingLogsFromEachLogTypes();
}

void ph::LogSettings::turnOnWritingLogsFromEachModule()
{
	setModuleNamesToWrite(
		{ "Audio", "Base", "Input", "Logs", "Physics", "Renderer", "Resources", "States", "Utilities", "World", "Terminal", "None" }
	);
}

void ph::LogSettings::turnOnWritingLogsFromEachLogTypes() 
{
	const std::size_t count = static_cast<std::size_t>(LogType::Count);
	mLogTypesToWrite.clear();
	mLogTypesToWrite.resize(count);
	for (std::size_t i = 0; i < count; ++i)
		mLogTypesToWrite[i] = static_cast<LogType>(i);
}

void ph::LogSettings::addLogType(const LogType& logTypeName)
{
	for (std::size_t i = 0; i < mLogTypesToWrite.size(); ++i)
		if (mLogTypesToWrite[i] == logTypeName) return;
	mLogTypesToWrite.emplace_back(logTypeName);		
}

void ph::LogSettings::addModuleName(const std::string& moduleName)
{
	for (std::size_t i = 0; i < mModuleNamesToWrite.size(); ++i)
		if (mModuleNamesToWrite[i] == moduleName) return;
	mModuleNamesToWrite.emplace_back(moduleName);
}

void ph::LogSettings::setWritingLogs(bool enabled)
{
	mShouldLogIntoConsole = enabled;
	mShouldLogIntoFile = enabled;
}

bool ph::LogSettings::shouldBeWrittenIntoConsole(const LogData& log) const
{
	return mShouldLogIntoConsole && shouldBeWritten(log);
}

bool ph::LogSettings::shouldBeWrittenIntoFile(const LogData& log) const
{
	return mShouldLogIntoFile && shouldBeWritten(log);
}

bool ph::LogSettings::shouldBeWritten(const LogData& log) const
{
	return shouldBeWrittenConsideringLogType(log) && shouldBeWrittenConsideringModuleName(log);
}

bool ph::LogSettings::shouldBeWrittenConsideringLogType(const LogData& log) const
{
	for (LogType type : mLogTypesToWrite) 
		if (type == log.type) 
			return true;
	return false;
}

bool ph::LogSettings::shouldBeWrittenConsideringModuleName(const LogData& log) const
{
	for (const std::string& moduleName : mModuleNamesToWrite)
		if (moduleName == log.moduleName)
			return true;
	return false;
}

