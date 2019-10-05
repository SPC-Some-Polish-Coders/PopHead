#pragma once

#include "event.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

class GameCloser;

void handleGlobalKeyboardShortcuts(sf::Window&, GameCloser&, const ph::Event&);

}