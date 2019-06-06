#include "commandPrompt.hpp"

#include "Base/gameData.hpp"

using PopHead::Cmd::CommandPrompt;

CommandPrompt::CommandPrompt()
	:mCommandPromptBackground(sf::Vector2f(1000, 100))
	,mIsVisible(false)
{
	mCommandPromptBackground.setFillColor(sf::Color(0, 0, 0, 230));
}

void CommandPrompt::input()
{
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Tab))
		mIsVisible = !mIsVisible;
}

void CommandPrompt::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mIsVisible)
		target.draw(mCommandPromptBackground, states);
}