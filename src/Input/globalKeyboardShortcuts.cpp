#include "globalKeyboardShortcuts.hpp"
#include "gameData.hpp"

namespace ph {

GlobalKeyboardShortcuts::GlobalKeyboardShortcuts()
	:mWindowSizeState(WindowSizeState::fullScreen)
	,mGameData(nullptr)
{
}

void GlobalKeyboardShortcuts::handleEvent(const sf::Event& e)
{
	if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F11)
		handleWindowMinimalizeAndMaximalizeShortcut();
}

void GlobalKeyboardShortcuts::handleWindowMinimalizeAndMaximalizeShortcut()
{
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

void GlobalKeyboardShortcuts::update()
{
	handleCloseGameShortcut();
}

void GlobalKeyboardShortcuts::handleCloseGameShortcut()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::LSystem) 
		&& sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
		mGameData->getGameCloser().closeGame();
}

}
