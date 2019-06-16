#include "terminalImage.hpp"
#include "Base/gameData.hpp"

ph::TerminalImage::TerminalImage(TerminalSharedData terminalSharedData)
	:mTerminalSharedData(terminalSharedData)
	,mTerminalBackground(sf::Vector2f(650, 200))
{
	mTerminalBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mTerminalBackground.setPosition(-450.f, 300.f);
}

void ph::TerminalImage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mTerminalSharedData->mIsVisible) {
		target.draw(mTerminalBackground, states);
		target.draw(mTerminalSharedData->mText, states);
	}
}

void ph::TerminalImage::move(sf::Vector2f offset)
{
	mTerminalBackground.move(offset);
	mTerminalSharedData->mText.move(offset);
}

void ph::TerminalImage::initializeText(GameData* gameData)
{
	gameData->getFonts().load("fonts/consolab.ttf");
	auto& text = mTerminalSharedData->mText;
	text.setFont(gameData->getFonts().get("fonts/consolab.ttf"));
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(18);
	text.setPosition(-450.f, 300.f);
}
