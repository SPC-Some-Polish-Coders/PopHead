#include "iniLoader.hpp"
#include "Logs/LogsIniLoader/logsIniLoader.hpp"
#include "Logs/logger.hpp"
#include <iostream>
#include <string>

using PopHead::Logs::LogData;
using PopHead::Logs::LogType;
using PopHead::Logs::LogsIniLoader;
using PopHead::Utilities::IniLoader;

std::fstream IniLoader::iniSettingsFile;
std::string IniLoader::currentLine;

void IniLoader::openTheFile()
{
	iniSettingsFile.open("logs/settings.ini", std::ios::in);
	if (!iniSettingsFile.is_open()) std::cout << "\n.ini file could not be opened\n";
}

void IniLoader::closeTheFile()
{
	iniSettingsFile.close();
	if (iniSettingsFile.is_open()) std::cout << "\n.ini file could not be closed\n";
}

bool IniLoader::findPhrase(std::string searchedPhrase)
{
	while (std::getline(iniSettingsFile, currentLine)) {
		if (currentLine.find(searchedPhrase) != std::string::npos)
			return true;
		if (iniSettingsFile.eof())
		{
			//PH_EXCEPTION("HELLO!"); I don't know why but instead of throwing runtime error, program just freezes
			closeTheFile();
			return false;
		}
	}
}

bool IniLoader::findValue(std::string searchedValue)
{
	if (currentLine.find(searchedValue) != std::string::npos)
		return true;
	else
		return false;
}

bool IniLoader::iniGetShouldLogIntoConsole()
{
	openTheFile();
	return LogsIniLoader::getShouldLogIntoConsole();
	closeTheFile();
}

bool IniLoader::iniGetShouldLogIntoFile()
{
	openTheFile();
	return LogsIniLoader::getShouldLogIntoFile();
	closeTheFile();
}

std::vector<LogType> IniLoader::iniGetLogTypesToWrite()
{
	openTheFile();
	return LogsIniLoader::getLogTypesToWrite();
}

std::vector<std::string> IniLoader::iniGetModuleNamesToWrite()
{
	openTheFile();
	return LogsIniLoader::getModuleNamesToWrite();
}




