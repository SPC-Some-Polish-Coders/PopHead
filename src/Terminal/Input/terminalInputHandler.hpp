#ifndef POPHEAD_TERMINAL_KEYBOARDINPUTHANDLER_H_
#define POPHEAD_TERMINAL_KEYBOARDINPUTHANDLER_H_

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

	void handleInput();
private:
	void handleKeyboardCharactersInput();
	void handleBackspace();
	void handleEnter();
	void handleLastCommandShortcut();
	void clearTextShortcut();
	void showOrHideCommandPromptInput();
	inline void setKeyRepeatEnabled(bool enabled);

private:
	GameData* mGameData;
	TerminalSharedData mTerminalSharedData;
	std::string& mContent;
	int mIndexOfCurrentLastCommand;
	bool mIsEnterClicked;
};

}

#endif // !POPHEAD_COMMANDPROMPT_KEYBOARDINPUTHANDLER_H_
