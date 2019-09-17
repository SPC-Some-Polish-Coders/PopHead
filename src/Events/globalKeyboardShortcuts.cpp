#include "globalKeyboardShortcuts.hpp"
#include "gameData.hpp"

namespace ph {

void handleGlobalKeyboardShortcuts(sf::RenderWindow& renderWindow, GameCloser& gameCloser, const ph::Event& phEvent)
{
	if(auto* e = std::get_if<sf::Event>(&phEvent))
	{
		if(e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::F11)
		{
			auto windowSize = renderWindow.getSize();

			if(windowSize == sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height))
				renderWindow.create(sf::VideoMode(1000, 750), "PopHead", sf::Style::Default);
			else
				renderWindow.create(sf::VideoMode(), "PopHead", sf::Style::Fullscreen);
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::LSystem) 
		&& sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
		gameCloser.closeGame();
}

}
