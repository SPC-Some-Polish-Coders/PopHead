#include "terminalImage.hpp"
#include "Base/gameData.hpp"

ph::TerminalImage::TerminalImage(TerminalSharedData terminalSharedData)
	:mTerminalSharedData(terminalSharedData)
	,mBackground({650, 400})
	,mSeparatorBetweenInputAndOutputArea({650, 10})
{
	mBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mBackground.setPosition(-450.f, 100.f);
	mSeparatorBetweenInputAndOutputArea.setFillColor(sf::Color::Black);
	mSeparatorBetweenInputAndOutputArea.setPosition(-450, 125);
}

void ph::TerminalImage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mTerminalSharedData->mIsVisible) {
		target.draw(mBackground);
		target.draw(mTerminalSharedData->mText);
		target.draw(mSeparatorBetweenInputAndOutputArea);
		target.draw(mOutputArea);
	}
}

void ph::TerminalImage::move(sf::Vector2f offset)
{
	mBackground.move(offset);
	mTerminalSharedData->mText.move(offset);
	mSeparatorBetweenInputAndOutputArea.move(offset);
	mOutputArea.move(offset);
}

void ph::TerminalImage::initializeText(GameData* gameData)
{
	gameData->getFonts().load("fonts/consolab.ttf");
	auto& text = mTerminalSharedData->mText;
	text.setFont(gameData->getFonts().get("fonts/consolab.ttf"));
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(18);
	text.setPosition(-450.f, 100.f);
}
