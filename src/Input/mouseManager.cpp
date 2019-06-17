#include "mouseManager.hpp"
#include "gameData.hpp"
#include "eventLoop.hpp"

auto ph::MouseManager::getMousePosition() const -> sf::Vector2i
{
    return sf::Mouse::getPosition( mGameData->getRenderer().getWindow() );
}

bool ph::MouseManager::hasMouseJustMoved() const
{
    return EventLoop::hasMouseJustMoved();
}

bool ph::MouseManager::isMouseButtonPressed(sf::Mouse::Button button) const
{
    return sf::Mouse::isButtonPressed(button);
}

bool ph::MouseManager::isMouseButtonJustPressed(sf::Mouse::Button button) const
{
    return (EventLoop::isMouseButtonJustPressed() && EventLoop::getMouseButton() == button);
}

bool ph::MouseManager::isMouseButtonJustReleased(sf::Mouse::Button button) const
{
    return (EventLoop::isMouseButtonJustReleased() && EventLoop::getMouseButton() == button);
}
