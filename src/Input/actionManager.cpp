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

}

void ActionManager::deleteKeyFromAction ( const std::string& toWhichAction, sf::Keyboard::Key button )
{

}

void ActionManager::clearAction(const std::string& action)
{

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
