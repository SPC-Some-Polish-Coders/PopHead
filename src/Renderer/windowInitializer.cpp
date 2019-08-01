#include "Renderer/windowInitializer.hpp"
#include "Logs/logs.hpp"

namespace ph {

sf::VideoMode WindowInitializer::getWindowSize()
{
	return sf::VideoMode(getWindowWidth(), getWindowHeight());
}

sf::Uint32 WindowInitializer::getStyle()
{
	return getFullscreenMode() ? sf::Style::Fullscreen : sf::Style::Default;
}

int WindowInitializer::getWindowWidth()
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
			int width = getResolutionValue();
			closeTheFile();
			return width;
		}
	}
	PH_LOG_ERROR("Window width wasn't found in file");
	return 0;   // TODO: change to another default value
}

int WindowInitializer::getWindowHeight()
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
			int height = getResolutionValue();
			closeTheFile();
			return height;
		}
	}
	PH_LOG_ERROR("Window height wasn't found in file");
	return 0;   // TODO: change to another default value
}

int WindowInitializer::getResolutionValue()
{
	std::size_t equalSignPosition = currentLine.find("=") + 1;
	currentLine = currentLine.substr(equalSignPosition, 4);
	return std::stoi(currentLine);
}

bool WindowInitializer::getFullscreenMode()
{
	openTheFile();
	if (findPhrase("FullscreenMode="))
	{
		return getBool(currentLine);
	}
	PH_LOG_ERROR("Value of fullscreen mode was not found in file");
	return false;
}

}