#include "actionEventManager.hpp"
#include <algorithm>
#include "Logs/logs.hpp"

namespace ph {

void ActionEventManager::init()
{
	mEnabled = true;

	//TODO: loading player's favorite controls from file

	//setting up default moving actions
	addAction("movingUp", {sf::Keyboard::W, sf::Keyboard::Up});
	addAction("movingDown", {sf::Keyboard::S, sf::Keyboard::Down});
	addAction("movingRight", {sf::Keyboard::D, sf::Keyboard::Right});
	addAction("movingLeft", {sf::Keyboard::A, sf::Keyboard::Left});
	addAction("use", sf::Keyboard::E);
	addAction("changeWeapon", sf::Keyboard::Q);
	addAction("gunAttack", sf::Keyboard::Enter);
	addAction("meleeAttack", sf::Keyboard::BackSlash);
	addAction("pauseScreen", sf::Keyboard::Escape);
}

void ActionEventManager::addAction(const std::string& action, std::vector<sf::Keyboard::Key> buttons)
{
	auto last = std::unique(buttons.begin(), buttons.end());
	buttons.erase(last, buttons.end());
	mActions[action] = Action{buttons, true};
	PH_LOG_INFO("Action was added to ActionEventManager.");
}

void ActionEventManager::addAction(const std::string& action, sf::Keyboard::Key button)
{
	mActions[action] = {std::vector<sf::Keyboard::Key>{button}, true};
	PH_LOG_INFO("Action was added to ActionEventManager.");
}

void ActionEventManager::addKeyToAction(const std::string& action, sf::Keyboard::Key button)
{
	auto found = mActions.find(action);

	if(found != mActions.end()) {
		auto& vec = found->second.keys;
		if(std::find(vec.begin(), vec.end(), button) == vec.end())
			vec.emplace_back(button);
	}
	PH_LOG_INFO("Key was added to action.");
}

void ActionEventManager::deleteKeyFromAction(const std::string& action, sf::Keyboard::Key button)
{
	auto found = mActions.find(action);

	if(found != mActions.end()) {
		auto& vec = found->second.keys;
		vec.erase(std::remove(vec.begin(), vec.end(), button), vec.end());
	}
	PH_LOG_INFO("Key was deleted from action.");
}

void ActionEventManager::deleteAction(const std::string& action)
{
	auto found = mActions.find(action);
	mActions.erase(found);
	PH_LOG_INFO("Action was deleted from ActionEventManager.");
}
void ActionEventManager::setActionEnabled(const std::string& action, bool enabled)
{
	auto found = mActions.find(action);
	if(found != mActions.end())
		found->second.enabled = enabled;
}

void ActionEventManager::setAllActionsEnabled(bool enabled)
{
	for(auto& action : mActions)
		action.second.enabled = enabled;
}

void ActionEventManager::clearAllActions() noexcept
{
	mActions.clear();
	PH_LOG_INFO("All actions were cleared.");
}

void ActionEventManager::setEnabled(bool enabled)
{
	mEnabled = enabled;
}

bool ActionEventManager::isActionPressed(const std::string& action)
{
	if(!mEnabled)
		return false;

	for(const auto& button : mActions[action].keys)
		if(sf::Keyboard::isKeyPressed(button))
			return true;
	return false;
}

void ActionEventManager::addActionEventsTo(std::vector<ActionEvent>& actionEvents, const sf::Event currentSfmlEvent)
{
	if(currentSfmlEvent.type == sf::Event::KeyPressed || currentSfmlEvent.type == sf::Event::KeyReleased)
	{
		for(const auto& action : mActions) 
		{
			if(action.second.enabled) 
			{
				auto actionKeys = action.second.keys;
				auto found = std::find(actionKeys.begin(), actionKeys.end(), currentSfmlEvent.key.code);
				if(found != actionKeys.end()) {
					ActionEvent actionEvent(
						action.first,
						currentSfmlEvent.type == sf::Event::KeyPressed ? ActionEvent::Pressed : ActionEvent::Released
					);
					actionEvents.emplace_back(actionEvent);
				}
			}
		}
	}
}

}
