#include "Logs/logger.hpp"
#include "iniLoader.hpp"
#include <iostream>
#include <string>

using PopHead::Utilities::IniLoader;

std::fstream IniLoader::iniSettingsFile;
std::string IniLoader::currentLine;

void IniLoader::openTheFile()
{
	iniSettingsFile.open("logs/settings.ini", std::ios::in);
	if (!iniSettingsFile.is_open())
	{
		throw std::runtime_error("'settings.ini' file could not be opened!");
		//PH_EXCEPTION("'settings.ini' file could not be opened!");
		//TODO: MAKE 'PH_EXCEPTION' WORK IN THIS PART OF PROGRAM
	}
}

void IniLoader::closeTheFile()
{
	iniSettingsFile.close();
	if (iniSettingsFile.is_open())
	{
		throw std::runtime_error("'settings.ini' file could not be closed!");
		//PH_EXCEPTION("'settings.ini' file could not be closed!");
	}
}

bool IniLoader::findPhrase(std::string searchedPhrase)
{
	while (std::getline(iniSettingsFile, currentLine)) {
		if (currentLine.find(searchedPhrase) != std::string::npos)
			return true;
		if (iniSettingsFile.eof())
		{
			throw std::runtime_error("' " + searchedPhrase + "' in settings.ini file could not be found!");
			//PH_EXCEPTION("' " + searchedPhrase + "' in settings.ini file could not be found!");
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




