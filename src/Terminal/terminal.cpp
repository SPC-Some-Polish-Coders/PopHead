#include "terminal.hpp"

#include "gameData.hpp"

ph::Terminal::Terminal()
	:mTerminalSharedData(new TerminalData())
	,mTerminalImage(mTerminalSharedData)
	,mKeyboardInputHandler(mTerminalSharedData)
	,mGameData(nullptr)
{
}

void ph::Terminal::input()
{
	mKeyboardInputHandler.handleInput();

	if(mKeyboardInputHandler.isEnterClicked()) {
		auto& content = mTerminalSharedData->mContent;
		auto& lastCommands = mTerminalSharedData->mLastCommands;

		if (content.size() != 0)
		{
		lastCommands.emplace_front(content);
		if (lastCommands.size() > 10)
			lastCommands.pop_back();
		}

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

void ph::Terminal::pushOutputLine(const OutputLine& line)
{
	mTerminalImage.getOutputArea().pushOutputLine(line);
}
