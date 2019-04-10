#include "actionManager.hpp"
#include "Base/gameData.hpp"
#include <algorithm>

using PopHead::Input::ActionManager;

ActionManager::ActionManager()
{
///TODO: loading player's favorite controls from file
}

void ActionManager::addAction( const std::string& action, std::vector<sf::Keyboard::Key>& buttons )
{
    auto last = std::unique(buttons.begin(), buttons.end());
    buttons.erase(last, buttons.end());
    mActions[action] = buttons;
}

void ActionManager::addAction( const std::string& action, sf::Keyboard::Key button )
{
    mActions[action] = std::vector<sf::Keyboard::Key>{button};
}

void ActionManager::addKeyToAction( const std::string& action, sf::Keyboard::Key button )
{
    auto found = mActions.find(action);

    if(found != mActions.end()){
        auto& vec = found->second;
        if(std::find(vec.begin(), vec.end(), button) == vec.end())
            found->second.emplace_back(button);
    }
}

void ActionManager::deleteKeyFromAction( const std::string& action, sf::Keyboard::Key button )
{
    auto found = mActions.find(action);

    if(found != mActions.end()){
        auto& vec = found->second;
        vec.erase(std::remove(vec.begin(), vec.end(), button), vec.end());
    }
}

void ActionManager::deleteAction(const std::string& action)
{
    auto found = mActions.find(action);
    mActions.erase(found);
}

bool ActionManager::isActionPressed( const std::string& action )
{
    for(const auto& button : mActions[action]){
        if(sf::Keyboard::isKeyPressed(button))
            return true;
    }
    return false;
}

bool ActionManager::isActionJustPressed( const std::string& action )
{
    for(const auto& button : mActions[action]){
        if(isKeyJust(sf::Event::KeyPressed, button))
            return true;
    }
    return false;
}

bool ActionManager::isActionJustReleased( const std::string& action )
{
    for(const auto& button : mActions[action]){
        if(isKeyJust(sf::Event::KeyReleased, button))
            return true;
    }
    return false;
}

bool ActionManager::isKeyJust(sf::Event::EventType type, sf::Keyboard::Key key) const
{
    sf::Event event;
    mGameData->getRenderer().getWindow().pollEvent( event );
    return(event.type == type && event.key.code == key);
}
