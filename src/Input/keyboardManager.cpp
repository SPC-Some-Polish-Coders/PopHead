#include "Input/keyboardManager.hpp"

#include "Base/gameData.hpp"

using PopHead::Input::KeyboardManager;


bool KeyboardManager::isKeyPressed( sf::Keyboard::Key key ) const
{
    return sf::Keyboard::isKeyPressed(key);
}

bool KeyboardManager::isKeyJustPressed( sf::Keyboard::Key key )
{
    sf::Event event;

    if(mGameData->getRenderer().getWindow().pollEvent( event ));

	return sf::Keyboard::isKeyPressed(key);
}
