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
	if(e.type != sf::Event::KeyPressed)
		return;

	if(e.key.code == sf::Keyboard::Tab) {
		showOrHideCommandPrompt();
		return;
	}

	if(mTerminalSharedData->mIsVisible)
	{
		switch(e.key.code)
		{
			case sf::Keyboard::A: mContent += "a"; break;
			case sf::Keyboard::B: mContent += "b"; break;
			case sf::Keyboard::C: mContent += "c"; break;
			case sf::Keyboard::D: mContent += "d"; break;
			case sf::Keyboard::E: mContent += "e"; break;
			case sf::Keyboard::F: mContent += "f"; break;
			case sf::Keyboard::G: mContent += "g"; break;
			case sf::Keyboard::H: mContent += "h"; break;
			case sf::Keyboard::I: mContent += "i"; break;
			case sf::Keyboard::J: mContent += "j"; break;
			case sf::Keyboard::K: mContent += "k"; break;
			case sf::Keyboard::L: mContent += "l"; break;
			case sf::Keyboard::M: mContent += "m"; break;
			case sf::Keyboard::N: mContent += "n"; break;
			case sf::Keyboard::O: mContent += "o"; break;
			case sf::Keyboard::P: mContent += "p"; break;
			case sf::Keyboard::Q: mContent += "q"; break;
			case sf::Keyboard::R: mContent += "r"; break;
			case sf::Keyboard::S: mContent += "s"; break;
			case sf::Keyboard::T: mContent += "t"; break;
			case sf::Keyboard::U: mContent += "u"; break;
			case sf::Keyboard::V: mContent += "v"; break;
			case sf::Keyboard::W: mContent += "w"; break;
			case sf::Keyboard::X: mContent += "x"; break;
			case sf::Keyboard::Y: mContent += "y"; break;
			case sf::Keyboard::Z: mContent += "z"; break;
			case sf::Keyboard::Num1: mContent += "1"; break;
			case sf::Keyboard::Num2: mContent += "2"; break;
			case sf::Keyboard::Num3: mContent += "3"; break;
			case sf::Keyboard::Num4: mContent += "4"; break;
			case sf::Keyboard::Num5: mContent += "5"; break;
			case sf::Keyboard::Num6: mContent += "6"; break;
			case sf::Keyboard::Num7: mContent += "7"; break;
			case sf::Keyboard::Num8: mContent += "8"; break;
			case sf::Keyboard::Num9: mContent += "9"; break;
			case sf::Keyboard::Num0: mContent += "0"; break;
			case sf::Keyboard::Hyphen: mContent += "-"; break;
			case sf::Keyboard::Space: mContent += " "; break;

			case sf::Keyboard::BackSpace:
				if(mContent.size() > 0)
					mContent.pop_back();
				break;

			case sf::Keyboard::Enter:
				mIsEnterClicked = true;
				updateLastCommands();
				break;

			case sf::Keyboard::Up:
				if(mIndexOfCurrentLastCommand + 1 < static_cast<int>(mTerminalSharedData->mLastCommands.size()))
				{
					++mIndexOfCurrentLastCommand;
					if(mIndexOfCurrentLastCommand >= 0)
						mContent = mTerminalSharedData->mLastCommands[mIndexOfCurrentLastCommand];
				}
				break;
			case sf::Keyboard::Down:
				if(mIndexOfCurrentLastCommand > -1)
				{
					--mIndexOfCurrentLastCommand;
					if(mIndexOfCurrentLastCommand == -1)
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
