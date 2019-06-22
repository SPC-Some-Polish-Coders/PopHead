#pragma once

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
	auto getSharedData() -> TerminalSharedData & { return mTerminalSharedData; }

private:
	TerminalSharedData mTerminalSharedData;
	TerminalImage mTerminalImage;
	TerminalInputHandler mKeyboardInputHandler;
	CommandInterpreter mCommandInterpreter;
	GameData* mGameData;
};

}
