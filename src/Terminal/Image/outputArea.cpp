#include "outputArea.hpp"
#include "Base/gameData.hpp"

namespace
{
	constexpr unsigned int numberOfOutputLines = 20;
	constexpr float spaceBetweenTheNumbers = 14;
	constexpr unsigned int fontSize = 15;
}

void ph::OutputArea::init(GameData* gameData)
{
	float positionY = 135;
	for(int i = 0; i <= numberOfOutputLines; ++i, positionY += spaceBetweenTheNumbers) {
		sf::Text text("an output line", gameData->getFonts().get("fonts/consolab.ttf"), fontSize);
		text.setPosition(-450, positionY);
		mOutputLines.emplace_back(std::move(text));
	}
}

void ph::OutputArea::pushOutputText(const std::string& text)
{
	if(mContentOfLines.size() >= numberOfOutputLines)
		mContentOfLines.pop_back();

	mContentOfLines.emplace_front(text);

	for(int i = 0; i != mContentOfLines.size(); ++i)
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
