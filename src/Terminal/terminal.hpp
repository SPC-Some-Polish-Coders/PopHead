#ifndef POPHEAD_TERMINAL_COMMANDPROMPT_H_
#define POPHEAD_TERMINAL_COMMANDPROMPT_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "terminalSharedData.hpp"
#include "Input/terminalInputHandler.hpp"
#include "Commands/commandInterpreter.hpp"
#include "Image/terminalImage.hpp"

namespace ph {

class GameData;

class Terminal
{
public:
	Terminal();
	void init(GameData*);

	void input();
	void pushOutputLine(const OutputLine&);

	auto getImage() -> TerminalImage& { return mTerminalImage; }

private:
	TerminalSharedData mTerminalSharedData;
	TerminalImage mTerminalImage;
	TerminalInputHandler mKeyboardInputHandler;
	CommandInterpreter mCommandInterpreter;
	GameData* mGameData;
};

}

#endif // !POPHEAD_CMD_COMMANDPROMPT_H_
