#pragma once

namespace ph {

class GameData;

class GlobalKeyboardShortcuts
{
public:
	void setGameData(GameData* const gameData) { mGameData = gameData; }
	void handle() const;

private:
	void windowMinimalizeAndMaximalizeShortcut() const;

private:
	GameData* mGameData;
};

}