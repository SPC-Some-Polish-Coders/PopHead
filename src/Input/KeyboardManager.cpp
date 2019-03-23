#include "keyboardManager.hpp"

using PopHead::Input::KeyboardManager;


bool KeyboardManager::isKeyPressed( sf::Keyboard::Key key ) const
{
    return sf::Keyboard::isKeyPressed(key);
}

bool KeyboardManager::isKeyJustPressed( sf::Keyboard::Key key ) const
{

}
