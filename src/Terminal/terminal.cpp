#include "terminal.hpp"
#include "gameData.hpp"

namespace ph {

Terminal::Terminal()
	:mTerminalSharedData(new TerminalData())
	,mTerminalRenderer(mTerminalSharedData)
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
}

void Terminal::setSceneRegistry(entt::registry* reg)
{
	mCommandInterpreter.setSceneRegistry(reg);
}

void Terminal::handleEvent(const ph::Event& phEvent)
{
	if(auto* e = std::get_if<sf::Event>(&phEvent))
		mKeyboardInputHandler.handleEvent(*e);
}

void Terminal::update()
{
	if(mKeyboardInputHandler.isEnterClicked()) {
		auto& content = mTerminalSharedData->content;
		mCommandInterpreter.handleCommand(content);
		content.clear();
	}

	mTerminalRenderer.update();

	// TODO: Refactor this mess
	mKeyboardInputHandler.update();
}

void Terminal::pushOutputLine(const OutputLine& line)
{
	mTerminalRenderer.pushOutputLine(line);
}

}
