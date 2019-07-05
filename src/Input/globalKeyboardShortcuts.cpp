#include "globalKeyboardShortcuts.hpp"
#include "gameData.hpp"

namespace ph {

void GlobalKeyboardShortcuts::handle() const
{
	windowMinimalizeAndMaximalizeShortcut();
}

void GlobalKeyboardShortcuts::windowMinimalizeAndMaximalizeShortcut() const
{
	enum class WindowSizeState{fullScreen, notFullScreen};
	static WindowSizeState windowSizeState = WindowSizeState::fullScreen;

	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::F11)) {
		switch(windowSizeState)
		{
		case WindowSizeState::fullScreen:
			windowSizeState = WindowSizeState::notFullScreen;
			mGameData->getRenderer().getWindow().create(sf::VideoMode(1000, 750), "PopHead", sf::Style::Default);
			break;
		case WindowSizeState::notFullScreen:
			windowSizeState = WindowSizeState::fullScreen;
			mGameData->getRenderer().getWindow().create(sf::VideoMode(), "PopHead", sf::Style::Fullscreen);
			break;
		}
	}
}

}
