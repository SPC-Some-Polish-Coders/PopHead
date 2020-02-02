#pragma once

#include "event.hpp"
#include <SFML/Window/Window.hpp>

namespace ph {

void handleGlobalKeyboardShortcuts(sf::Window&, const ph::Event&);

}