#pragma once

#include "Terminal/terminalSharedData.hpp"
#include "Events/event.hpp"
#include <SFML/Window/Window.hpp>

namespace ph {

class TerminalInputHandler
{
public:
	TerminalInputHandler(TerminalSharedData, sf::Window&);
	
	bool isEnterClicked() { return mIsEnterClicked; }

	void handleEvent(Event&);
	void update();
private:
	void updateLastCommands();
	void showOrHideCommandPrompt();

private:
	TerminalSharedData mSharedData;
	sf::Window& mWindow;
	int mIndexOfCurrentLastCommand;
	bool mIsEnterClicked;
};

}
