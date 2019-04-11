#include "mouseManager.hpp"
#include "Base/gameData.hpp"

using PopHead::Input::MouseManager;


auto MouseManager::getMousePosition() const -> sf::Vector2i
{
    return sf::Mouse::getPosition( mGameData->getRenderer().getWindow() );
}

bool MouseManager::hasMouseMovedSinceLastFrame() const
{

}

bool MouseManager::isMouseButtonPressed(sf::Mouse::Button button) const
{
    ///1st implementation
    /*sf::Event event;
    mGameData->getRenderer().getWindow().pollEvent( event );
    return ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == button));*/

    return false;

    ///2nd implementation
    #if 0
    sf::Event event;
    while(mGameData->getRenderer().getWindow().pollEvent( event )){
        if(event.type == sf::Event::MouseButtonPressed){
            if(event.mouseButton.button == button)
                return true;
        }
    }
    return false;
    #endif // 0

    ///BOTH IMPLEMENTATION DON'T WORK CORRECTLY
}

bool MouseManager::isMouseButtonReleased(sf::Mouse::Button) const
{

}
