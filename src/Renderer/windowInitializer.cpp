#include "Renderer/windowInitializer.hpp"

sf::VideoMode ph::WindowInitializer::getWindowSize()
{
	return sf::VideoMode(getWindowWidth(), getWindowHeight());
}

sf::Uint32 ph::WindowInitializer::getStyle()
{
	return getFullscreenMode() ? sf::Style::Fullscreen : sf::Style::Default;
}

int ph::WindowInitializer::getWindowWidth()
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
}

int ph::WindowInitializer::getWindowHeight()
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
}

int ph::WindowInitializer::getResolutionValue()
{
	std::size_t equalSignPosition = currentLine.find("=") + 1;
	currentLine = currentLine.substr(equalSignPosition, 4);
	return std::stoi(currentLine);
}

bool ph::WindowInitializer::getFullscreenMode()
{
	openTheFile();
	if (findPhrase("FullscreenMode="))
	{
		return getBool(currentLine);
	}
}