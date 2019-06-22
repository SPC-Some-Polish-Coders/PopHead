#pragma once

#include "Utilities/iniLoader.hpp"
#include "SFML/Graphics.hpp"

namespace ph{

class WindowInitializer : private IniLoader
{
public:
	static sf::VideoMode getWindowSize();
	static sf::Uint32 getStyle();
private:
	static int getWindowWidth();
	static int getWindowHeight();
	static int getResolutionValue();
	static bool getFullscreenMode();
};

}
