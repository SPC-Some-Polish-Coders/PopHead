#ifndef POPHEAD_TERMINAL_KEYBOARDINPUTHANDLER_H_
#define POPHEAD_TERMINAL_KEYBOARDINPUTHANDLER_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Terminal/terminalSharedData.hpp"

namespace PopHead {
	namespace Base { class GameData; }
	namespace Input { struct KeyboardManager; }
namespace Terminal {

class TerminalInputHandler
{
public:
	TerminalInputHandler(TerminalSharedData);
	
	void setGameData(Base::GameData* gameData) { mGameData = gameData; }

	void handleInput();
private:
	void handleKeyboardCharactersInput();
	void handleBackspace();
	void handleEnter();
	void showOrHideCommandPromptInput();

private:
	Base::GameData* mGameData;
	TerminalSharedData mCommandPromptSharedData;
	std::string& mContent;
};

}}

#endif // !POPHEAD_COMMANDPROMPT_KEYBOARDINPUTHANDLER_H_

