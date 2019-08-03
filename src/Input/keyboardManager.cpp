#include "Input/keyboardManager.hpp"
#include "eventLoop.hpp"
#include <algorithm>

namespace ph {

bool KeyboardManager::isKeyPressed( sf::Keyboard::Key key ) const
{
    return sf::Keyboard::isKeyPressed(key);
}

bool KeyboardManager::isKeyJustPressed( sf::Keyboard::Key key ) const
{
    return (EventLoop::isKeyJustPressed() && EventLoop::getPressedKey() == key);
}

bool KeyboardManager::isKeyJustReleased( sf::Keyboard::Key key ) const
{
    return (EventLoop::isKeyJustReleased() && EventLoop::getReleasedKey() == key);
}

}