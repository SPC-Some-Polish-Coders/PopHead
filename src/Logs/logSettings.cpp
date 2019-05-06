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
	setModulesToWrite(
		{ "Base", "Inputs", "Logs", "Music", "Physics", "Renderer", "Resources", "Sound", "States", "None" }
	);
}

void LogSettings::setWritingLogsFromEachLogTypes() 
{
	setTypesToWrite( { LogType::ERROR, LogType::INFO, LogType::WARNING } );
}

bool LogSettings::shouldThisLogBeWrittenIntoConsole(const Log& log) const
{
	return mShouldLogIntoConsole && shouldThisLogBeWritten(log);
}

bool LogSettings::shouldThisLogBeWrittenIntoFile(const Log& log) const
{
	return mShouldLogIntoFile && shouldThisLogBeWritten(log);
}

bool LogSettings::shouldThisLogBeWritten(const Log& log) const
{
	return shouldThisLogBeWrittenConsideringLogType(log) && shouldThisLogBeWrittenConsideringModuleID(log);
}

bool LogSettings::shouldThisLogBeWrittenConsideringLogType(const Log& log) const
{
	for (auto type : mTypesToWrite) {
		if (type == log.type) {
			return true;
		}
	}
	return false;
}

bool LogSettings::shouldThisLogBeWrittenConsideringModuleID(const Log& log) const
{
	for (auto moduleName : mModulesToWrite){
		if (moduleName == log.moduleName){
			return true;
		}
	}
	return false;
}