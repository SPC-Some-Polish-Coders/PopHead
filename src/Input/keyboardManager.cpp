#include "Input/keyboardManager.hpp"
#include "eventLoop.hpp"

bool ph::KeyboardManager::isKeyPressed( sf::Keyboard::Key key ) const
{
    return sf::Keyboard::isKeyPressed(key);
}

bool ph::KeyboardManager::isKeyJustPressed( sf::Keyboard::Key key ) const
{
    return (EventLoop::isKeyJustPressed() && EventLoop::getKey() == key);
}

bool ph::KeyboardManager::isKeyJustReleased( sf::Keyboard::Key key ) const
{
    return (EventLoop::isKeyJustReleased() && EventLoop::getKey() == key);
}
