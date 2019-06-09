#ifndef POPHEAD_COMMANDPROMPT_KEYBOARDINPUTHANDLER_H_
#define POPHEAD_COMMANDPROMPT_KEYBOARDINPUTHANDLER_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "CommandPrompt/commandPromptSharedData.hpp"

namespace PopHead {
	namespace Base { class GameData; }
	namespace Input { struct KeyboardManager; }
namespace CommandPrompt {

class KeyboardInputHandler
{
public:
	KeyboardInputHandler(CommandPromptSharedData);
	
	void setGameData(Base::GameData* gameData) { mGameData = gameData; }

	void handleInput();
private:
	void handleKeyboardCharactersInput();
	void handleBackspace();
	void handleEnter();
	void showOrHideCommandPromptInput();

private:
	Base::GameData* mGameData;
	CommandPromptSharedData mCommandPromptSharedData;
	std::string& mContent;
};

}}

#endif // !POPHEAD_COMMANDPROMPT_KEYBOARDINPUTHANDLER_H_

