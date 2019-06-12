#include "logsInitializer.hpp"
#include "Logs/logger.hpp"

bool ph::LogsInitializer::getShouldLogIntoConsole()
{
	openTheFile();
	if (findPhrase("ShouldBeWrittenIntoConsole="))
	{
		bool returnVal = getBool(currentLine);
		closeTheFile();
		return returnVal;
	}
}

bool ph::LogsInitializer::getShouldLogIntoFile()
{
	openTheFile();
	if (findPhrase("ShouldBeWrittenIntoFile="))
	{
		bool returnVal = getBool(currentLine);
		closeTheFile();
		return returnVal;
	}
}

std::vector<ph::LogType> ph::LogsInitializer::getLogTypesToWrite()
{
	openTheFile();
	std::vector<LogType> InitLogTypesToWrite;
	if (findPhrase("LogTypesToWrite="))
	{
		if (findValue("All"))
			InitLogTypesToWrite = { LogType::Warning, LogType::Error, LogType::Info, LogType::FromUser, LogType::Count };
		else
		{
			if (findValue("Error")) InitLogTypesToWrite.emplace_back(LogType::Error);
			if (findValue("Info")) InitLogTypesToWrite.emplace_back(LogType::Info);
			if (findValue("Warning")) InitLogTypesToWrite.emplace_back(LogType::Warning);
			if (findValue("From user")) InitLogTypesToWrite.emplace_back(LogType::FromUser);
		}
	}
	closeTheFile();
	return InitLogTypesToWrite;
}

std::vector<std::string> ph::LogsInitializer::getModuleNamesToWrite()
{
	openTheFile();
	std::vector<std::string> InitModuleNamesToWrite;
	if (findPhrase("ModuleNamesToWrite="))
	{
		if (findValue("All"))
			InitModuleNamesToWrite = { "Audio", "Base", "Input", "Logs", "Physics",
			"Renderer", "Resources", "States", "Utilities", "World", "Terminal", "None" };
		else
		{
			if (findValue("Audio")) InitModuleNamesToWrite.emplace_back("Audio");
			if (findValue("Base")) InitModuleNamesToWrite.emplace_back("Base");
			if (findValue("Input")) InitModuleNamesToWrite.emplace_back("Input");
			if (findValue("Logs")) InitModuleNamesToWrite.emplace_back("Logs");
			if (findValue("Physics")) InitModuleNamesToWrite.emplace_back("Physics");
			if (findValue("Renderer")) InitModuleNamesToWrite.emplace_back("Renderer");
			if (findValue("Resources")) InitModuleNamesToWrite.emplace_back("Resources");
			if (findValue("States")) InitModuleNamesToWrite.emplace_back("States");
			if (findValue("Utilities")) InitModuleNamesToWrite.emplace_back("Utilities");
			if (findValue("World")) InitModuleNamesToWrite.emplace_back("World");
			if (findValue("Terminal")) InitModuleNamesToWrite.emplace_back("Terminal");
			if (findValue("None")) InitModuleNamesToWrite.emplace_back("None");
		}
	}
	closeTheFile();
	return InitModuleNamesToWrite;
}


