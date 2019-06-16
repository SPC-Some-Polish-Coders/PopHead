#include "outputArea.hpp"

ph::OutputArea::OutputArea()
{
}

void ph::OutputArea::pushOutputText(const std::string& text)
{
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
