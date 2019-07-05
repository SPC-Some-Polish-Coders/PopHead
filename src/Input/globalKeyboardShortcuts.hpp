#pragma once

namespace ph {

class GameData;

class GlobalKeyboardShortcuts
{
public:
	GlobalKeyboardShortcuts();

	void setGameData(GameData* const gameData) { mGameData = gameData; }
	void handleShortcuts();

private:
	void windowMinimalizeAndMaximalizeShortcut();

private:
	GameData* mGameData;

	enum class WindowSizeState{fullScreen, notFullScreen};
	WindowSizeState mWindowSizeState;
};

}