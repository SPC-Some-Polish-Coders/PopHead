#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Terminal/terminalSharedData.hpp"

namespace ph {

class GameData;
struct KeyboardManager;

class TerminalInputHandler
{
public:
	TerminalInputHandler(TerminalSharedData);
	
	void setGameData(GameData* gameData) { mGameData = gameData; }
	bool isEnterClicked() { return mIsEnterClicked; }

	void handleEvent(const sf::Event&);
	void update();
private:
	void updateLastCommands();
	void showOrHideCommandPrompt();

private:
	GameData* mGameData;
	TerminalSharedData mTerminalSharedData;
	std::string& mContent;
	int mIndexOfCurrentLastCommand;
	bool mIsEnterClicked;
};

}
