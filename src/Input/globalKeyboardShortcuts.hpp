#pragma once

#include <SFML/Graphics.hpp>

namespace ph {

class GameCloser;

void handleGlobalKeyboardShortcuts(sf::RenderWindow&, GameCloser&, const sf::Event&);

}