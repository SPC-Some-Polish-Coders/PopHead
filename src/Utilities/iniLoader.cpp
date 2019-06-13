#include "Logs/logger.hpp"
#include "iniLoader.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>
#include <sstream>

std::fstream ph::IniLoader::iniSettingsFile;
std::string ph::IniLoader::currentLine;

// WARNING: Don't use PH_EXCEPTION or PH_LOG if Logger constructor is using this method because it can result in recursion

void ph::IniLoader::handleException(const std::string& message)
{
	std::cout << message << std::endl;
	throw std::runtime_error(message);
}

void ph::IniLoader::openTheFile()
{
	iniSettingsFile.open("config.ini", std::ios::in);
	if (!iniSettingsFile.is_open())
	{
		handleException("[IniLoader::openTheFile] 'config.ini' file could not be opened!");
	}
}

void ph::IniLoader::closeTheFile()
{
	iniSettingsFile.close();
	if (iniSettingsFile.is_open())
	{
		handleException("[IniLoader::closeTheFile] 'config.ini' file could not be closed!");
	}
}

bool ph::IniLoader::findPhrase(const std::string& searchedPhrase)
{
	while (std::getline(iniSettingsFile, currentLine)) {
		if (currentLine.find(searchedPhrase) != std::string::npos)
			return true;
		if (iniSettingsFile.eof())
		{
			handleException("[IniLoader::findPhrase] ' " + searchedPhrase + "' phrase in settings.ini file could not be found!");
		}
	}
}

bool ph::IniLoader::findValue(const std::string& searchedValue)
{
	if (currentLine.find(searchedValue) != std::string::npos)
		return true;
	else
		return false;
}

bool ph::IniLoader::getBool(const std::string& currentLine)
{
	if (findValue("true")) return true;
	else if (findValue("false")) return false;
	else
	{
		std::cout << "[IniLoader::getBool] No specified logical value detected for '" + currentLine + "'. Assumed 'true'" << std::endl;
		return true;
	}
}
