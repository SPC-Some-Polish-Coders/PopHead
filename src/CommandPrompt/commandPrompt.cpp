#include "commandPrompt.hpp"

#include "Base/gameData.hpp"

using PopHead::CommandPrompt::CommandPrompt;

CommandPrompt::CommandPrompt()
	:mCommandPromptBackground(sf::Vector2f(650, 200))
	,mIsVisible(false)
	,mText(new sf::Text())
	,mContent(new std::string())
	,mKeyboardInputHandler(mText, mContent)
{
	mCommandPromptBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mCommandPromptBackground.setPosition(-450.f, 300.f);
}

void CommandPrompt::input()
{
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Tab))
		mIsVisible = !mIsVisible;

	mKeyboardInputHandler.handleInput();
}

void CommandPrompt::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mIsVisible) {
		target.draw(mCommandPromptBackground, states);
		target.draw(*mText, states);
	}
}

void CommandPrompt::move(sf::Vector2f offset)
{
	mCommandPromptBackground.move(offset);
	mText->move(offset);
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
	mText->setFont(mGameData->getFonts().get("fonts/consolab.ttf"));
	mText->setFillColor(sf::Color::White);
	mText->setCharacterSize(18);
	mText->setString("PopHead command prompt:");
	mText->setPosition(-450.f, 300.f);
}
