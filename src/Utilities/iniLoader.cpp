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

bool IniLoader::findPhrase(const std::string& searchedPhrase)
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
		std::cout << "No specified logical value detected for '" + currentLine + "' . Assumed 'true'";
		//PH_LOG(LogType::Warning, "No specified logical value detected for '" + currentLine + "' . Assumed 'true'");
		return true;
	}
}

bool IniLoader::iniGetFullscreenMode()
{
	openTheFile();
	if (findPhrase("FullscreenMode="))
	{
		return getBool(currentLine);
	}
}

int IniLoader::iniGetWindowWidth()
{
	openTheFile();
	if (findPhrase("WindowWidth="))
	{
		if (findValue("Auto"))
		{
			closeTheFile();
			return sf::VideoMode::getDesktopMode().width;
		}
		else
		{
			std::size_t equalSignPosition = currentLine.find("=") + 1;
			currentLine = currentLine.substr(equalSignPosition, 4);	//These three lines may become a function in order to keep the code cleaner
			int width = std::stoi(currentLine);
			closeTheFile();
			return width;
		}
	}
}

int IniLoader::iniGetWindowHeight()
{
	openTheFile();
	if (findPhrase("WindowHeight="))
	{
		if (findValue("Auto"))
		{
			closeTheFile();
			return sf::VideoMode::getDesktopMode().height;
		}
		else
		{
			std::size_t equalSignPosition = currentLine.find("=") + 1;
			currentLine = currentLine.substr(equalSignPosition, 4);
			int height = std::stoi(currentLine);
			closeTheFile();
			return height;
		}
	}
}




