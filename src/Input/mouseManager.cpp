#include "mouseManager.hpp"
#include "Base/gameData.hpp"
#include "eventLoop.hpp"

using PopHead::Input::MouseManager;


auto MouseManager::getMousePosition() const -> sf::Vector2i
{
    return sf::Mouse::getPosition( mGameData->getRenderer().getWindow() );
}

bool MouseManager::hasMouseMovedSinceLastInput() const
{
    return EventLoop::hasMouseMoved();
}

bool MouseManager::isMouseButtonPressed(sf::Mouse::Button button) const
{
    return (EventLoop::isMouseButtonPressed() && EventLoop::getMouseButton() == button);
}

bool MouseManager::isMouseButtonReleased(sf::Mouse::Button button) const
{
    return (EventLoop::isMouseButtonReleased() && EventLoop::getMouseButton() == button);
}
