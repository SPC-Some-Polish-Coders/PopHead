#include "terminalImage.hpp"
#include "terminalStyleConstants.hpp"
#include "Renderer/renderer.hpp"
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

void TerminalImage::draw() const
{
	if(mTerminalSharedData->mIsVisible) {
		Renderer::submitSFMLObject(mBackground);
		Renderer::submitSFMLObject(mTerminalSharedData->mInputLine);
		Renderer::submitSFMLObject(mSeparatorBetweenInputAndOutputArea);
		Renderer::submitSFMLObject(mOutputArea);
	}
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