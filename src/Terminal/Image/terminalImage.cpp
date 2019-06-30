#include "terminalImage.hpp"
#include "terminalStyleConstants.hpp"
#include "gameData.hpp"


namespace ph {

using namespace TerminalStyleConstants;

TerminalImage::TerminalImage(TerminalSharedData terminalSharedData)
	:mTerminalSharedData(terminalSharedData)
	,mBackground(terminalSize)
	,mSeparatorBetweenInputAndOutputArea(separatorBetweenInputAndOutputAreaSize)
{
	mBackground.setFillColor(terminalColor);
	mBackground.setPosition(terminalPosition);
	mSeparatorBetweenInputAndOutputArea.setFillColor(sf::Color::Black);
	mSeparatorBetweenInputAndOutputArea.setPosition(separatorBetweenInputAndOutputAreaPosition);
}

void TerminalImage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mTerminalSharedData->mIsVisible) {
		target.draw(mBackground);
		target.draw(mTerminalSharedData->mInputLine);
		target.draw(mSeparatorBetweenInputAndOutputArea);
		target.draw(mOutputArea);
	}
}

void TerminalImage::move(sf::Vector2f offset)
{
	mBackground.move(offset);
	mTerminalSharedData->mInputLine.move(offset);
	mSeparatorBetweenInputAndOutputArea.move(offset);
	mOutputArea.move(offset);
}

void TerminalImage::init(GameData* gameData)
{
	initializeText(gameData);
	mOutputArea.init(gameData);
}

void TerminalImage::initializeText(GameData* gameData)
{
	auto& text = mTerminalSharedData->mInputLine;
	text.setFont(gameData->getFonts().get(inputFontPath));
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(inputCharacterSize);
	text.setPosition(inputTextPosition);
}

}