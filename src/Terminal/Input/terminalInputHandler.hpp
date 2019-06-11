#ifndef POPHEAD_TERMINAL_KEYBOARDINPUTHANDLER_H_
#define POPHEAD_TERMINAL_KEYBOARDINPUTHANDLER_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Terminal/terminalSharedData.hpp"

namespace ph {
	namespace Base { class GameData; }
	namespace Input { struct KeyboardManager; }
namespace Terminal {

class TerminalInputHandler
{
public:
	TerminalInputHandler(TerminalSharedData);
	
	void setGameData(Base::GameData* gameData) { mGameData = gameData; }
	bool isEnterClicked() { return mIsEnterClicked; }

	void handleInput();
private:
	void handleKeyboardCharactersInput();
	void handleBackspace();
	void handleEnter();
	void showOrHideCommandPromptInput();

private:
	Base::GameData* mGameData;
	TerminalSharedData mTerminalSharedData;
	std::string& mContent;
	bool mIsEnterClicked;
};

}}

#endif // !POPHEAD_COMMANDPROMPT_KEYBOARDINPUTHANDLER_H_

