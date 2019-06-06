#include "commandPrompt.hpp"

using PopHead::Cmd::CommandPrompt;

CommandPrompt::CommandPrompt()
	:mCommandPromptBackground(sf::Vector2f(1000, 100))
{
	mCommandPromptBackground.setFillColor(sf::Color(0, 0, 0, 230));
}

void CommandPrompt::input()
{

}

void CommandPrompt::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mCommandPromptBackground, states);
}