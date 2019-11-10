#include "TerminalInputHandler.hpp"
#include "Events/actionEventManager.hpp"
#include "gameData.hpp"

namespace ph {

TerminalInputHandler::TerminalInputHandler(TerminalSharedData terminalSharedData)
	:mTerminalSharedData(terminalSharedData)
	,mContent(mTerminalSharedData->mContent)
	,mIsEnterClicked(false)
	,mIndexOfCurrentLastCommand(-1)
	,mGameData(nullptr)
{
}

void TerminalInputHandler::handleEvent(const sf::Event& e)
{
	if(mTerminalSharedData->mIsVisible && e.type == sf::Event::TextEntered)
	{
		char key = static_cast<char>(e.text.unicode);
		if (!iscntrl(key))
			mContent += key;
	}

	if (e.type == sf::Event::KeyPressed)
	{
		switch (e.key.code)
		{
		case sf::Keyboard::Tab: 
			showOrHideCommandPrompt();
			break;

		case sf::Keyboard::BackSpace:
			if (mContent.size() > 0)
				mContent.pop_back();
			break;

		case sf::Keyboard::Enter:
			mIsEnterClicked = true;
			updateLastCommands();
			break;

		case sf::Keyboard::Up:
			if (mIndexOfCurrentLastCommand + 1 < static_cast<int>(mTerminalSharedData->mLastCommands.size()))
			{
				++mIndexOfCurrentLastCommand;
				if (mIndexOfCurrentLastCommand >= 0)
					mContent = mTerminalSharedData->mLastCommands[mIndexOfCurrentLastCommand];
			}
			break;
		case sf::Keyboard::Down:
			if (mIndexOfCurrentLastCommand > -1)
			{
				--mIndexOfCurrentLastCommand;
				if (mIndexOfCurrentLastCommand == -1)
					mContent.clear();
				else
					mContent = mTerminalSharedData->mLastCommands[mIndexOfCurrentLastCommand];
			}
		}
	}
}

void TerminalInputHandler::update()
{
	mTerminalSharedData->mInputLine.setString(mContent);

	mIsEnterClicked = false;
}

void TerminalInputHandler::updateLastCommands()
{
	mIndexOfCurrentLastCommand = -1;
	auto& content = mTerminalSharedData->mContent;
	auto& lastCommands = mTerminalSharedData->mLastCommands;
	if(content.size() != 0) {
		lastCommands.emplace_front(content);
		if(lastCommands.size() > 10)
			lastCommands.pop_back();
	}
}

void TerminalInputHandler::showOrHideCommandPrompt()
{
	bool& isVisible = mTerminalSharedData->mIsVisible;
	ActionEventManager::setEnabled(isVisible);
	isVisible = !isVisible;
	auto& window = mGameData->getRenderWindow();
	window.setKeyRepeatEnabled(isVisible);
}

}
