#include "terminal.hpp"

#include "Base/gameData.hpp"

ph::Terminal::Terminal()
	:mTerminalSharedData(new TerminalData())
	,mTerminalBackground(sf::Vector2f(650, 200))
	,mKeyboardInputHandler(mTerminalSharedData)
{
	mTerminalBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mTerminalBackground.setPosition(-450.f, 300.f);
}

void ph::Terminal::input()
{
	mKeyboardInputHandler.handleInput();

	if(mKeyboardInputHandler.isEnterClicked()) {
		auto& content = mTerminalSharedData->mContent;
		mCommandInterpreter.handleCommand(content);
		content.clear();
	}
}

void ph::Terminal::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mTerminalSharedData->mIsVisible) {
		target.draw(mTerminalBackground, states);
		target.draw(mTerminalSharedData->mText, states);
	}
}

void ph::Terminal::move(sf::Vector2f offset)
{
	mTerminalBackground.move(offset);
	mTerminalSharedData->mText.move(offset);
}

void ph::Terminal::init(GameData* gameData)
{
	mGameData = gameData;
	mKeyboardInputHandler.setGameData(mGameData);
	mCommandInterpreter.setGameData(mGameData);
	initializeText();
}

void ph::Terminal::initializeText()
{
	mGameData->getFonts().load("fonts/consolab.ttf");
	auto& text = mTerminalSharedData->mText;
	text.setFont(mGameData->getFonts().get("fonts/consolab.ttf"));
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(18);
	text.setString("PopHead command prompt:");
	text.setPosition(-450.f, 300.f);
}
