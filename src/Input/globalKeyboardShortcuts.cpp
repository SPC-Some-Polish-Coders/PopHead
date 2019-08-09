#include "globalKeyboardShortcuts.hpp"
#include "gameData.hpp"

namespace ph {

GlobalKeyboardShortcuts::GlobalKeyboardShortcuts()
	:mWindowSizeState(WindowSizeState::fullScreen)
{
}

void GlobalKeyboardShortcuts::handleShortcuts()
{
	handleWindowMinimalizeAndMaximalizeShortcut();
	handleCloseGameShortcut();
}

void GlobalKeyboardShortcuts::handleWindowMinimalizeAndMaximalizeShortcut()
{
	auto& keyboard = mGameData->getInput().getKeyboard();
	if(keyboard.isKeyJustPressed(sf::Keyboard::F11)) {
		switch(mWindowSizeState)
		{
		case WindowSizeState::fullScreen:
			mWindowSizeState = WindowSizeState::notFullScreen;
			mGameData->getRenderWindow().create(sf::VideoMode(1000, 750), "PopHead", sf::Style::Default);
			break;
		case WindowSizeState::notFullScreen:
			mWindowSizeState = WindowSizeState::fullScreen;
			mGameData->getRenderWindow().create(sf::VideoMode(), "PopHead", sf::Style::Fullscreen);
			break;
		}
	}
}

void GlobalKeyboardShortcuts::handleCloseGameShortcut()
{
	auto& keyboard = mGameData->getInput().getKeyboard();
	if(keyboard.isKeyPressed(sf::Keyboard::Escape))
		mGameData->getGameCloser().closeGame();
}

}
