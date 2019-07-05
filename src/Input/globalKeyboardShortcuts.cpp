#include "globalKeyboardShortcuts.hpp"
#include "gameData.hpp"

namespace ph {

GlobalKeyboardShortcuts::GlobalKeyboardShortcuts()
	:mWindowSizeState(WindowSizeState::fullScreen)
{
}

void GlobalKeyboardShortcuts::handleShortcuts()
{
	windowMinimalizeAndMaximalizeShortcut();
}

void GlobalKeyboardShortcuts::windowMinimalizeAndMaximalizeShortcut()
{
	auto& keyboard = mGameData->getInput().getKeyboard();
	if(keyboard.isKeyJustPressed(sf::Keyboard::F11)) {
		switch(mWindowSizeState)
		{
		case WindowSizeState::fullScreen:
			mWindowSizeState = WindowSizeState::notFullScreen;
			mGameData->getRenderer().getWindow().create(sf::VideoMode(1000, 750), "PopHead", sf::Style::Default);
			break;
		case WindowSizeState::notFullScreen:
			mWindowSizeState = WindowSizeState::fullScreen;
			mGameData->getRenderer().getWindow().create(sf::VideoMode(), "PopHead", sf::Style::Fullscreen);
			break;
		}
	}
}

}
