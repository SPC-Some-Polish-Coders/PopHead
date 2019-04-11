#include "Input/keyboardManager.hpp"
#include "eventLoop.hpp"

using PopHead::Input::KeyboardManager;


bool KeyboardManager::isKeyPressed( sf::Keyboard::Key key ) const
{
    return sf::Keyboard::isKeyPressed(key);
}

bool KeyboardManager::isKeyJustPressed( sf::Keyboard::Key key ) const
{
    return (EventLoop::isKeyPressed() && EventLoop::getKey() == key);
}

bool KeyboardManager::isKeyJustReleased( sf::Keyboard::Key key ) const
{
    return (EventLoop::isKeyReleased() && EventLoop::getKey() == key);
}
