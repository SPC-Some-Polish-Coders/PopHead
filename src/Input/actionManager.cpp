#include "actionManager.hpp"

using PopHead::Input::ActionManager;

ActionManager::ActionManager()
{
///TODO: loading player's favorite controls from file


}

void ActionManager::addAction ( const std::string& action, const std::vector<sf::Keyboard::Key>& buttons )
{
    mActions[action] = buttons;
}

void ActionManager::addAction ( const std::string& action, sf::Keyboard::Key button )
{
    mActions[action] = std::vector<sf::Keyboard::Key>{button};
}

void ActionManager::addKeyToAction ( const std::string& toWhichAction, sf::Keyboard::Key button )
{
    auto found = mActions.find(toWhichAction);
    if(found != mActions.end()){
        found->second.emplace_back(button);
    }
}

void ActionManager::deleteKeyFromAction ( const std::string& toWhichAction, sf::Keyboard::Key button )
{
    #if 0
    auto found = mActions.find(toWhichAction);

    if(found != mActions.end()){
        for( auto it = *found->second.begin(); it != *found->second.end(); ++it ){
            if(*it == button){
                *found->second.erase(it);
                break;
            }
        }
    }
    #endif // 0
}

void ActionManager::clearAction(const std::string& action)
{
    auto found = mActions.find(action);
    mActions.erase(found);
}

bool ActionManager::isActionPressed ( const std::string& action ) const
{
    return false;
}

bool ActionManager::isActionJustPressed ( const std::string& action ) const
{
    return false;
}

bool ActionManager::isActionJustReleased ( const std::string& action ) const
{
    return false;
}
