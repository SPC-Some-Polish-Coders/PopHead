#include "terminalImage.hpp"
#include "terminalStyleConstants.hpp"
#include "Base/gameData.hpp"

using namespace ph::TerminalStyleConstants;

ph::TerminalImage::TerminalImage(TerminalSharedData terminalSharedData)
	:mTerminalSharedData(terminalSharedData)
	,mBackground(terminalSize)
	,mSeparatorBetweenInputAndOutputArea(separatorBetweenInputAndOutputAreaSize)
{
	mBackground.setFillColor(terminalColor);
	mBackground.setPosition(terminalPosition);
	mSeparatorBetweenInputAndOutputArea.setFillColor(separatorBetweenInputAndOutputColor);
	mSeparatorBetweenInputAndOutputArea.setPosition(separatorBetweenInputAndOutputAreaPosition);
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
	gameData->getFonts().load(fontPath);
	auto& text = mTerminalSharedData->mInputLine;
	text.setFont(gameData->getFonts().get(fontPath));
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(inputCharacterSize);
	text.setPosition(inputTextPosition);
}
