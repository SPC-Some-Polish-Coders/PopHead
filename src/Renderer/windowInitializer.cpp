#include "Renderer/windowInitializer.hpp"

using PopHead::Renderer::WindowInitializer;
using PopHead::Utilities::IniLoader;

sf::RenderWindow WindowInitializer::getInitialWindow()
{
	if (iniGetFullscreenMode())
	{
		return sf::RenderWindow(sf::VideoMode(iniGetWindowWidth(), iniGetWindowHeight()), "PopHead", sf::Style::Fullscreen);
	}
	else 
		return sf::RenderWindow(sf::VideoMode(iniGetWindowWidth(), iniGetWindowHeight()), "PopHead", sf::Style::Default); //TODO: MAKE IT WORK
}

bool WindowInitializer::iniGetFullscreenMode()
{
	openTheFile();
	if (findPhrase("FullscreenMode="))
	{
		return getBool(currentLine);
	}
}

int WindowInitializer::iniGetWindowWidth()
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

int WindowInitializer::iniGetWindowHeight()
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