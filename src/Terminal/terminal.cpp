#include "terminal.hpp"

#include "gameData.hpp"

namespace ph {

Terminal::Terminal()
	:mTerminalSharedData(new TerminalData())
	,mTerminalImage(mTerminalSharedData)
	,mKeyboardInputHandler(mTerminalSharedData)
	,mGameData(nullptr)
{
}

void Terminal::init(GameData* gameData)
{
	mGameData = gameData;
	mKeyboardInputHandler.setGameData(mGameData);
	mCommandInterpreter.setGameData(mGameData);
	mCommandInterpreter.init();
	mTerminalImage.init(gameData);
}

void Terminal::input()
{
	mKeyboardInputHandler.handleInput();

	if(mKeyboardInputHandler.isEnterClicked()) {
		auto& content = mTerminalSharedData->mContent;
		mCommandInterpreter.handleCommand(content);
		content.clear();
	}
}

void Terminal::pushOutputLine(const OutputLine& line)
{
	mTerminalImage.getOutputArea().pushOutputLine(line);
}

}