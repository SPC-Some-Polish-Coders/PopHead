#include "actionManager.hpp"

using PopHead::Input::ActionManager;

ActionManager::ActionManager()
{

}

void ActionManager::addAction ( const std::string& action, std::vector<sf::Keyboard::Key> )
{

}

void ActionManager::addAction ( const std::string& action, sf::Keyboard::Key )
{

}

void ActionManager::addKeyToAction ( const std::string& toWhichAction, sf::Keyboard::Key )
{

}

void ActionManager::deleteKeyFromAction ( const std::string& toWhichAction, sf::Keyboard::Key )
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
