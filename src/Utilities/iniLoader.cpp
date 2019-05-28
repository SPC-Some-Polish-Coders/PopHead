#include "Logs/logger.hpp"
#include "iniLoader.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>
#include <sstream>

using PopHead::Utilities::IniLoader;

std::fstream IniLoader::iniSettingsFile;
std::string IniLoader::currentLine;

void IniLoader::openTheFile()
{
	iniSettingsFile.open("logs/settings.ini", std::ios::in);
	if (!iniSettingsFile.is_open())
	{
		// WARNING: Don't use PH_EXCEPTION or PH_LOG if Logger constructor is using this method becouse it can result in recursion
		std::cout<<"[IniLoader::openTheFile] 'settings.ini' file could not be opened!"<<std::endl;
		throw std::runtime_error("'settings.ini' file could not be opened!");
	}
}

void IniLoader::closeTheFile()
{
	iniSettingsFile.close();
	if (iniSettingsFile.is_open())
	{
		// WARNING: Don't use PH_EXCEPTION or PH_LOG if Logger constructor is using this method becouse it can result in recursion
		std::cout << "[IniLoader::closeTheFile] 'settings.ini' file could not be closed!" << std::endl;
		throw std::runtime_error("'settings.ini' file could not be closed!");
	}
}

bool IniLoader::findPhrase(const std::string& searchedPhrase)
{
	while (std::getline(iniSettingsFile, currentLine)) {
		if (currentLine.find(searchedPhrase) != std::string::npos)
			return true;
		if (iniSettingsFile.eof())
		{
			// WARNING: Don't use PH_EXCEPTION or PH_LOG if Logger constructor is using this method becouse it can result in recursion
			std::cout << "[IniLoader::findPhrase] ' " + searchedPhrase + "' in settings.ini file could not be found!" << std::endl;
			throw std::runtime_error("' " + searchedPhrase + "' in settings.ini file could not be found!");
			closeTheFile();
			return false;
		}
	}
}

bool IniLoader::findValue(const std::string& searchedValue)
{
	if (currentLine.find(searchedValue) != std::string::npos)
		return true;
	else
		return false;
}

bool IniLoader::getBool(const std::string& currentLine)
{
	if (findValue("1"))
	{
		closeTheFile();
		return true;
	}
	else if (findValue("0"))
	{
		closeTheFile();
		return false;
	}
	else
	{
		closeTheFile();
		// WARNING: Don't use PH_EXCEPTION or PH_LOG if Logger constructor is using this method becouse it can result in recursion
		std::cout << "[IniLoader::getBool] No specified logical value detected for '" + currentLine + "'.Assumed 'true'" << std::endl;
		return true;
	}
}
