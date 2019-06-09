#include "commandPrompt.hpp"

#include "Base/gameData.hpp"

using PopHead::CommandPrompt::CommandPrompt;

CommandPrompt::CommandPrompt()
	:mCommandPromptSharedData(new CommandPromptData())
	,mCommandPromptBackground(sf::Vector2f(650, 200))
	,mKeyboardInputHandler(mCommandPromptSharedData)
{
	mCommandPromptBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mCommandPromptBackground.setPosition(-450.f, 300.f);
}

void CommandPrompt::input()
{
	mKeyboardInputHandler.handleInput();
}

void CommandPrompt::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mCommandPromptSharedData->mIsVisible) {
		target.draw(mCommandPromptBackground, states);
		target.draw(mCommandPromptSharedData->mText, states);
	}
}

void CommandPrompt::move(sf::Vector2f offset)
{
	mCommandPromptBackground.move(offset);
	mCommandPromptSharedData->mText.move(offset);
}

void CommandPrompt::init(PopHead::Base::GameData* gameData)
{
	mGameData = gameData;
	mKeyboardInputHandler.setGameData(mGameData);
	initializeText();
}

void CommandPrompt::initializeText()
{
	mGameData->getFonts().load("fonts/consolab.ttf");
	auto& text = mCommandPromptSharedData->mText;
	text.setFont(mGameData->getFonts().get("fonts/consolab.ttf"));
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(18);
	text.setString("PopHead command prompt:");
	text.setPosition(-450.f, 300.f);
}
