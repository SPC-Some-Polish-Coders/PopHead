#include "terminal.hpp"

#include "Base/gameData.hpp"

ph::Terminal::Terminal()
	:mTerminalSharedData(new TerminalData())
	,mTerminalImage(mTerminalSharedData)
	,mKeyboardInputHandler(mTerminalSharedData)
{
}

void ph::Terminal::input()
{
	mKeyboardInputHandler.handleInput();

	if(mKeyboardInputHandler.isEnterClicked()) {
		auto& content = mTerminalSharedData->mContent;
		mTerminalSharedData->mLastCommand = content;
		mCommandInterpreter.handleCommand(content);
		content.clear();
	}
}

void ph::Terminal::init(GameData* gameData)
{
	mGameData = gameData;
	mKeyboardInputHandler.setGameData(mGameData);
	mCommandInterpreter.setGameData(mGameData);
	mTerminalImage.init(gameData);
}

void ph::Terminal::pushOutputLine(const std::string& line)
{
	mTerminalImage.getOutputArea().pushOutputText(line);
}
