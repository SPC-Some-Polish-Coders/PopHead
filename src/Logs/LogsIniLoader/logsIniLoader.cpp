#include "logsIniLoader.hpp"
#include "Logs/logger.hpp"

using PopHead::Logs::LogsIniLoader;
using PopHead::Logs::LogType;
using PopHead::Utilities::IniLoader;

bool LogsIniLoader::getShouldLogIntoConsole()
{
	openTheFile();
	if (findPhrase("ShouldBeWrittenIntoConsole="))
	{
		return getBool(currentLine);
	}
}

bool LogsIniLoader::getShouldLogIntoFile()
{
	openTheFile();
	if (findPhrase("ShouldBeWrittenIntoFile="))
	{
		return getBool(currentLine);
	}
}

std::vector<LogType> LogsIniLoader::getLogTypesToWrite()
{
	openTheFile();
	std::vector<LogType> InitLogTypesToWrite;
	if (findPhrase("LogTypesToWrite="))
	{
		if (findValue("All"))
			InitLogTypesToWrite = { LogType::Warning, LogType::Error, LogType::Info, LogType::Count };
		else
		{
			if (findValue("Error")) InitLogTypesToWrite.push_back(LogType::Error);
			if (findValue("Info")) InitLogTypesToWrite.push_back(LogType::Info);
			if (findValue("Warning")) InitLogTypesToWrite.push_back(LogType::Warning);
		}
	}
	closeTheFile();
	return InitLogTypesToWrite;
}

std::vector<std::string> LogsIniLoader::getModuleNamesToWrite()
{
	openTheFile();
	std::vector<std::string> InitModuleNamesToWrite;
	if (findPhrase("ModuleNamesToWrite="))
	{
		if (findValue("All"))
			InitModuleNamesToWrite = { "Audio", "Base", "Input", "Logs", "Physics",
			"Renderer", "Resources", "States", "Utilities", "World", "None" };
		else
		{
			if (findValue("Audio")) InitModuleNamesToWrite.push_back("Audio");
			if (findValue("Base")) InitModuleNamesToWrite.push_back("Base");
			if (findValue("Input")) InitModuleNamesToWrite.push_back("Input");
			if (findValue("Logs")) InitModuleNamesToWrite.push_back("Logs");
			if (findValue("Physics")) InitModuleNamesToWrite.push_back("Physics");
			if (findValue("Renderer")) InitModuleNamesToWrite.push_back("Renderer");
			if (findValue("Resources")) InitModuleNamesToWrite.push_back("Resources");
			if (findValue("States")) InitModuleNamesToWrite.push_back("States");
			if (findValue("Utilities")) InitModuleNamesToWrite.push_back("Utilities");
			if (findValue("World")) InitModuleNamesToWrite.push_back("World");
			if (findValue("None")) InitModuleNamesToWrite.push_back("None");
		}
	}
	closeTheFile();
	return InitModuleNamesToWrite;
}


