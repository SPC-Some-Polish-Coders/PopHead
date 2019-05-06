#include "logSettings.hpp"

using PopHead::Logs::LogSettings;
using PopHead::Logs::LogType;
using PopHead::Logs::Log;

LogSettings::LogSettings()
	:mShouldLogIntoConsole(true)
	,mShouldLogIntoFile(true)
{
	setWritingEachLog();
}

void LogSettings::setWritingLogs(bool enabled)
{
	mShouldLogIntoConsole = enabled;
	mShouldLogIntoFile = enabled;
}

void LogSettings::setWritingEachLog()
{
	setWritingLogsFromEachModules();
	setWritingLogsFromEachLogTypes();
}

void LogSettings::setWritingLogsFromEachModules()
{
	setModuleNamesToWrite(
		{ "Base", "Inputs", "Logs", "Music", "Physics", "Renderer", "Resources", "Sound", "States", "None" }
	);
}

void LogSettings::setWritingLogsFromEachLogTypes() 
{
	setTypesToWrite( { LogType::ERROR, LogType::INFO, LogType::WARNING } );
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
	for (auto type : mTypesToWrite) {
		if (type == log.type) {
			return true;
		}
	}
	return false;
}

bool LogSettings::shouldBeWrittenConsideringModuleName(const Log& log) const
{
	for (auto moduleName : mModuleNamesToWrite){
		if (moduleName == log.moduleName){
			return true;
		}
	}
	return false;
}