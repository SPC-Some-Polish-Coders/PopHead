#include "terminal.hpp"

namespace ph {

Terminal::Terminal(sf::Window& window)
	:mTerminalSharedData(new TerminalData())
	,mTerminalRenderer(mTerminalSharedData)
	,mKeyboardInputHandler(mTerminalSharedData, window)
{
}

void Terminal::init(SceneManager* sceneManager)
{
	mCommandInterpreter.init(sceneManager, mTerminalSharedData);
}

void Terminal::handleEvent(Event& e)
{
	mKeyboardInputHandler.handleEvent(e);
}

void Terminal::update(float dt)
{
	if(mKeyboardInputHandler.isEnterClicked()) {
		auto& content = mTerminalSharedData->content;
		mCommandInterpreter.handleCommand(content);
		content.clear();
	}

	mCommandInterpreter.update(dt);
	mTerminalRenderer.update();

	// TODO: Refactor this mess
	mKeyboardInputHandler.update();
}

void Terminal::pushOutputLine(const OutputLine& line)
{
	mTerminalRenderer.pushOutputLine(line);
}

}
