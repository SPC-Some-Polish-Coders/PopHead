#include "terminalImage.hpp"
#include "Base/gameData.hpp"

namespace
{
	constexpr float terminalStartXPosition = -480.f;
	constexpr float terminalWidth = 960;
}

ph::TerminalImage::TerminalImage(TerminalSharedData terminalSharedData)
	:mTerminalSharedData(terminalSharedData)
	,mBackground({terminalWidth, 400})
	,mSeparatorBetweenInputAndOutputArea({terminalWidth, 10})
{
	mBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mBackground.setPosition(terminalStartXPosition, 100.f);
	mSeparatorBetweenInputAndOutputArea.setFillColor(sf::Color::Black);
	mSeparatorBetweenInputAndOutputArea.setPosition(terminalStartXPosition, 125);
}

void ph::TerminalImage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mTerminalSharedData->mIsVisible) {
		target.draw(mBackground);
		target.draw(mTerminalSharedData->mInputLine);
		target.draw(mSeparatorBetweenInputAndOutputArea);
		target.draw(mOutputArea);
	}
}

void ph::TerminalImage::move(sf::Vector2f offset)
{
	mBackground.move(offset);
	mTerminalSharedData->mInputLine.move(offset);
	mSeparatorBetweenInputAndOutputArea.move(offset);
	mOutputArea.move(offset);
}

void ph::TerminalImage::init(GameData* gameData)
{
	initializeText(gameData);
	mOutputArea.init(gameData);
}

void ph::TerminalImage::initializeText(GameData* gameData)
{
	//TODO: Place where fonts are loaded shouldn't be here!
	gameData->getFonts().load("fonts/consolab.ttf");
	auto& text = mTerminalSharedData->mInputLine;
	text.setFont(gameData->getFonts().get("fonts/consolab.ttf"));
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(18);
	text.setPosition(terminalStartXPosition + 5, 100.f);
}
