#pragma once

#include <SFML/Window.hpp>
#include <map>

namespace ph {

struct KeyboardManager
{
    bool isKeyPressed( sf::Keyboard::Key ) const;
    bool isKeyJustPressed( sf::Keyboard::Key ) const;
    bool isKeyJustReleased( sf::Keyboard::Key ) const;
};

}
