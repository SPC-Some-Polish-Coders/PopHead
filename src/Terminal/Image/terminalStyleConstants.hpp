#pragma once

#include <SFML/Graphics.hpp>

namespace ph{

namespace TerminalStyleConstants
{
	constexpr float terminalWidth = 455.f;
	constexpr float terminalHeight = 200.f;
	const sf::Vector2f terminalSize(terminalWidth, terminalHeight);

	constexpr float terminalXPosition = -240.f;
	constexpr float terminalYPosition = 50.f;
	const sf::Vector2f terminalPosition(terminalXPosition, terminalYPosition);

	const sf::Color terminalColor(0, 0, 0, 230);

	constexpr float textXposition = terminalXPosition + 5.f;
	const sf::Vector2f inputTextPosition(textXposition, terminalYPosition);

	const sf::Vector2f separatorBetweenInputAndOutputAreaSize(terminalWidth, 5);
	const sf::Vector2f separatorBetweenInputAndOutputAreaPosition(terminalXPosition, terminalYPosition + 13);

	const std::string fontPath("fonts/tahoma.ttf");
	constexpr unsigned int inputCharacterSize = 10;
	constexpr unsigned int outputCharacterSize = 8;

	constexpr unsigned int numberOfOutputLines = 19;
	constexpr float spaceBetweenTheLines = 9;
}

}