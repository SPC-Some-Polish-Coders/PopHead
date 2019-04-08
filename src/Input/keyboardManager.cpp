#include "Input/keyboardManager.hpp"

#include "Base/gameData.hpp"

using PopHead::Input::KeyboardManager;


bool KeyboardManager::isKeyPressed( sf::Keyboard::Key key ) const
{
    return sf::Keyboard::isKeyPressed(key);
}

bool KeyboardManager::isKeyJustPressed( sf::Keyboard::Key key ) const
{
    return isKeyJust(sf::Event::KeyPressed ,key);
}

bool KeyboardManager::isKeyJustReleased( sf::Keyboard::Key key ) const
{
    return isKeyJust(sf::Event::KeyReleased ,key);
}

bool KeyboardManager::isKeyJust(sf::Event::EventType type, sf::Keyboard::Key key) const
{
    sf::Event event;
    mGameData->getRenderer().getWindow().pollEvent( event );
    return(event.type == type && event.key.code == key);
}
