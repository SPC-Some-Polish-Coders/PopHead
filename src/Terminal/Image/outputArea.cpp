#include "outputArea.hpp"
#include "terminalStyleConstants.hpp"
#include "gameData.hpp"
#include "Logs/log.hpp"

using namespace ph::TerminalStyleConstants;

void ph::OutputArea::init(GameData* gameData)
{
	float positionY = 135;
	for(int i = 0; i < numberOfOutputLines; ++i, positionY += spaceBetweenTheLines) {
		sf::Text text("", gameData->getFonts().get(fontPath), outputCharacterSize);
		text.setPosition(textXposition, positionY);
		mTexts.emplace_back(std::move(text));
	}
}

void ph::OutputArea::pushOutputLine(const OutputLine& line)
{
	if(mOutputLines.size() >= numberOfOutputLines)
		mOutputLines.pop_back();

	mOutputLines.emplace_front(line);

	for(size_t i = 0; i < mOutputLines.size(); ++i) {
		mTexts[i].setString(mOutputLines[i].mText);
		mTexts[i].setFillColor(mOutputLines[i].mColor);
	}
}

void ph::OutputArea::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(auto& line : mTexts)
		target.draw(line);
}

void ph::OutputArea::move(sf::Vector2f offset)
{
	for(auto& line : mTexts)
		line.move(offset);
}
