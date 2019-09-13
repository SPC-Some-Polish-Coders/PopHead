#pragma once

#include <SFML/Window.hpp>

namespace ph {

class GameData;

class GlobalKeyboardShortcuts
{
public:
	GlobalKeyboardShortcuts();

	void setGameData(GameData* const gameData) { mGameData = gameData; }

	void handleEvent(const sf::Event&);
	void update();

private:
	void handleWindowMinimalizeAndMaximalizeShortcut();
	void handleCloseGameShortcut();

private:
	GameData* mGameData;

	enum class WindowSizeState{fullScreen, notFullScreen};
	WindowSizeState mWindowSizeState;
};

}