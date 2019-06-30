#include "logSettings.hpp"
#include "Logs/LogsInitializer/logsInitializer.hpp"
#include <fstream>
#include <Utilities/debug.hpp>

namespace ph {

LogSettings::LogSettings()
	:mShouldLogIntoConsole(LogsInitializer::getShouldLogIntoConsole())
	,mShouldLogIntoFile(LogsInitializer::getShouldLogIntoFile())
	,mShouldLogIntoTerminal(LogsInitializer::getShouldLogIntoTerminal())
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

bool LogSettings::shouldBeWrittenIntoTerminal(const LogData& log) const
{
	return mShouldLogIntoTerminal && shouldBeWritten(log);
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

void LogSettings::addToVector(const LogType& logTypeName)
{
	if (mLogTypesToWrite.size() == 0)
	{
		mLogTypesToWrite.emplace_back(logTypeName);
	}
	else
		for (std::size_t i = 0; i < mLogTypesToWrite.size(); ++i)
		{
			if (mLogTypesToWrite[i] == logTypeName) return;
			mLogTypesToWrite.emplace_back(logTypeName);
		}
}

void LogSettings::addToVector(const std::string & moduleName)
{
	if (mModuleNamesToWrite.size() == 0)
	{
		mModuleNamesToWrite.emplace_back(moduleName);
		PH_LOG(LogType::Info, moduleName + " was added to displayed modules.");
	}
	else
		for (std::size_t i = 0; i < mModuleNamesToWrite.size() + 1; ++i)
		{
			if (mModuleNamesToWrite[i] == moduleName) return;
			mModuleNamesToWrite.emplace_back(moduleName);
			PH_LOG(LogType::Info, moduleName + " was added to displayed modules.");
		}
}

}