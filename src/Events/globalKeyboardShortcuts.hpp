#pragma once

#include "event.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

class GameCloser;

void handleGlobalKeyboardShortcuts(sf::RenderWindow&, GameCloser&, const ph::Event&);

}