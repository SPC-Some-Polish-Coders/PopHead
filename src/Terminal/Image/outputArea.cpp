#include "outputArea.hpp"
#include "terminalStyleConstants.hpp"
#include "Base/gameData.hpp"

using namespace ph::TerminalStyleConstants;

void ph::OutputArea::init(GameData* gameData)
{
	float positionY = 135;
	for(int i = 0; i < numberOfOutputLines; ++i, positionY += spaceBetweenTheLines) {
		sf::Text text("", gameData->getFonts().get(fontPath), outputCharacterSize);
		text.setPosition(terminalXPosition + 5, positionY);
		mOutputLines.emplace_back(std::move(text));
	}
}

void ph::OutputArea::pushOutputText(const std::string& text)
{
	if(mContentOfLines.size() >= numberOfOutputLines)
		mContentOfLines.pop_back();

	mContentOfLines.emplace_front(text);

	for(int i = 0; i < mContentOfLines.size(); ++i)
		mOutputLines[i].setString(mContentOfLines[i]);
}

void ph::OutputArea::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(auto& line : mOutputLines)
		target.draw(line);
}

void ph::OutputArea::move(sf::Vector2f offset)
{
	for(auto& line : mOutputLines)
		line.move(offset);
}
