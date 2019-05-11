#include "logSettings.hpp"
#include <fstream>
#include <iostream>
using PopHead::Logs::LogSettings;
using PopHead::Logs::LogType;
using PopHead::Logs::LogData;

std::fstream LogSettings::InitLogSettings::mSettingsFile;

LogSettings::LogSettings()
	: mShouldLogIntoConsole(LogSettings::InitLogSettings::mGetShouldLogIntoConsole()),
	mShouldLogIntoFile(LogSettings::InitLogSettings::mGetShouldLogIntoFile())

{
	setLogTypesToWrite(LogSettings::InitLogSettings::mGetLogTypesToWrite());
	setModuleNamesToWrite(LogSettings::InitLogSettings::mGetModuleNamesToWrite());
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

void LogSettings::InitLogSettings::mOpenTheFile()
{
	mSettingsFile.open("logs/settings.ini");
	if (!mSettingsFile.is_open()) std::cout << "\n.ini file could not be opened\n";
}

void LogSettings::InitLogSettings::mCloseTheFile()
{
	mSettingsFile.close();
	mSettingsFile.flush();
	if (mSettingsFile.is_open()) std::cout << "\n.ini file could not be closed\n";
}

bool LogSettings::InitLogSettings::mGetShouldLogIntoConsole()
{
	mOpenTheFile();
	std::string found;
	while (std::getline(mSettingsFile, found)) {
		if (found.find("ShouldBeWrittenIntoConsole=") != std::string::npos)
		{
			if (found.find("1") != std::string::npos) 	return true;
			else if (found.find("0") != std::string::npos)	return false;
			else 
			{
				std::cout << "\nBad format for '" << found << "' - [true] assumed\n";
				return true;
			}
		}
	}
	mCloseTheFile();
}

bool LogSettings::InitLogSettings::mGetShouldLogIntoFile()
{
	mOpenTheFile();
	std::string found;
	while (std::getline(mSettingsFile, found)) {
		if (found.find("ShouldBeWrittenIntoFile=") != std::string::npos)
		{
			if (found.find("1") != std::string::npos) 	return true;
			else if (found.find("0") != std::string::npos)	return false;
			else
			{
				std::cout << "\nBad format for '" << found << "' - [true] assumed\n";
				return true;
			}
		}
	}
	mCloseTheFile();
}

std::vector<LogType> LogSettings::InitLogSettings::mGetLogTypesToWrite()
{
	mOpenTheFile();
	std::string found;
	std::vector<LogType> InitLogTypesToWrite;
	while (std::getline(mSettingsFile, found)) {
		if (found.find("LogTypesToWrite=") != std::string::npos)
		{
			if (found.find("All") != std::string::npos)
				InitLogTypesToWrite = { LogType::Warning, LogType::Error, LogType::Info, LogType::Count };
			else
			{
				if (found.find("Error") != std::string::npos) InitLogTypesToWrite.push_back(LogType::Error);
				if (found.find("Info") != std::string::npos) InitLogTypesToWrite.push_back(LogType::Info);
				if (found.find("Warning") != std::string::npos) InitLogTypesToWrite.push_back(LogType::Warning);
			}
		}
	}
	mCloseTheFile();
	return InitLogTypesToWrite;
}

std::vector<std::string> LogSettings::InitLogSettings::mGetModuleNamesToWrite()
{
	mOpenTheFile();
	std::string found;
	std::vector<std::string> InitModuleNamesToWrite;
	while (std::getline(mSettingsFile, found))
	{
		if(found.find("ModuleNamesToWrite=") != std::string::npos) 
			if(found.find("All") != std::string::npos)
				InitModuleNamesToWrite = { "Audio", "Base", "Input", "Logs", "Physics", "Renderer", "Resources", "States", "Utilities", "World", "None" };
			else
			{
				if (found.find("Audio") != std::string::npos) InitModuleNamesToWrite.push_back("Audio");
				if (found.find("Base") != std::string::npos) InitModuleNamesToWrite.push_back("Base");
				if (found.find("Input") != std::string::npos) InitModuleNamesToWrite.push_back("Input");
				if (found.find("Logs") != std::string::npos) InitModuleNamesToWrite.push_back("Logs");
				if (found.find("Physics") != std::string::npos) InitModuleNamesToWrite.push_back("Physics");
				if (found.find("Renderer") != std::string::npos) InitModuleNamesToWrite.push_back("Renderer");
				if (found.find("Resources") != std::string::npos) InitModuleNamesToWrite.push_back("Resources");
				if (found.find("States") != std::string::npos) InitModuleNamesToWrite.push_back("States");
				if (found.find("Utilities") != std::string::npos) InitModuleNamesToWrite.push_back("Utilities");
				if (found.find("World") != std::string::npos) InitModuleNamesToWrite.push_back("World");
				if (found.find("None") != std::string::npos) InitModuleNamesToWrite.push_back("None");
			}
		}
	mCloseTheFile();
	return InitModuleNamesToWrite;
}
