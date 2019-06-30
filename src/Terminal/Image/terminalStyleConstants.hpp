#pragma once

#include <SFML/Graphics.hpp>

namespace ph{

namespace TerminalStyleConstants
{
	constexpr float terminalWidth = 640.f;
	constexpr float terminalHeight = 200.f;
	const sf::Vector2f terminalSize(terminalWidth, terminalHeight);

	constexpr float terminalXPosition = -320.f;
	constexpr float terminalYPosition = 50.f;
	const sf::Vector2f terminalPosition(terminalXPosition, terminalYPosition);

	const sf::Color terminalColor(0, 0, 0, 230);

	constexpr float textXposition = terminalXPosition + 5.f;
	const sf::Vector2f inputTextPosition(textXposition, terminalYPosition);

	const sf::Vector2f separatorBetweenInputAndOutputAreaSize(terminalWidth, 5);
	const sf::Vector2f separatorBetweenInputAndOutputAreaPosition(terminalXPosition, terminalYPosition + 15);

	const std::string inputFontPath("fonts/joystixMonospace.ttf");
	constexpr unsigned int inputCharacterSize = 10;
	const std::string outputFontPath("fonts/consola.ttf");
	constexpr unsigned int outputCharacterSize = 9;

	constexpr unsigned int numberOfOutputLines = 12;
	constexpr float spaceBetweenTheLines = 14;
}

}