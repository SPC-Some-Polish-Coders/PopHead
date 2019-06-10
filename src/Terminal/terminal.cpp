#include "terminal.hpp"

#include "Base/gameData.hpp"

using PopHead::Terminal::Terminal;

Terminal::Terminal()
	:mTerminalSharedData(new TerminalData())
	,mTerminalBackground(sf::Vector2f(650, 200))
	,mKeyboardInputHandler(mTerminalSharedData)
{
	mTerminalBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mTerminalBackground.setPosition(-450.f, 300.f);
}

void Terminal::input()
{
	mKeyboardInputHandler.handleInput();
}

void Terminal::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mTerminalSharedData->mIsVisible) {
		target.draw(mTerminalBackground, states);
		target.draw(mTerminalSharedData->mText, states);
	}
}

void Terminal::move(sf::Vector2f offset)
{
	mTerminalBackground.move(offset);
	mTerminalSharedData->mText.move(offset);
}

void Terminal::init(PopHead::Base::GameData* gameData)
{
	mGameData = gameData;
	mKeyboardInputHandler.setGameData(mGameData);
	initializeText();
}

void Terminal::initializeText()
{
	mGameData->getFonts().load("fonts/consolab.ttf");
	auto& text = mTerminalSharedData->mText;
	text.setFont(mGameData->getFonts().get("fonts/consolab.ttf"));
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(18);
	text.setString("PopHead command prompt:");
	text.setPosition(-450.f, 300.f);
}
