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

void Terminal::handleEvent(const ph::Event& phEvent)
{
	if(auto* e = std::get_if<sf::Event>(&phEvent))
		mKeyboardInputHandler.handleEvent(*e);
}

void Terminal::update()
{
	if(mKeyboardInputHandler.isEnterClicked()) {
		auto& content = mTerminalSharedData->mContent;
		mCommandInterpreter.handleCommand(content);
		content.clear();
	}

	mTerminalImage.draw();

	// TODO: Refactor this mess
	mKeyboardInputHandler.update();
}

void Terminal::pushOutputLine(const OutputLine& line)
{
	mTerminalImage.getOutputArea().pushOutputLine(line);
}

}
