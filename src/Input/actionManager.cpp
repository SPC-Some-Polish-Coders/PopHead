#include "actionManager.hpp"
#include "eventLoop.hpp"
#include <algorithm>
#include "Logs/logs.hpp"

namespace ph {

ActionManager::ActionManager()
{
	///TODO: loading player's favorite controls from file

	//setting up default moving actions
	addAction("movingUp", {sf::Keyboard::W, sf::Keyboard::Up});
	addAction("movingDown", {sf::Keyboard::S, sf::Keyboard::Down});
	addAction("movingRight", {sf::Keyboard::D, sf::Keyboard::Right});
	addAction("movingLeft", {sf::Keyboard::A, sf::Keyboard::Left});
	addAction("attack", sf::Keyboard::Space);
}

void ActionManager::addAction(const std::string& action, std::vector<sf::Keyboard::Key> buttons)
{
	auto last = std::unique(buttons.begin(), buttons.end());
	buttons.erase(last, buttons.end());
	mActions[action] = buttons;
	PH_LOG_INFO("Action was added to ActionManager.");
}

void ActionManager::addAction(const std::string& action, sf::Keyboard::Key button)
{
	mActions[action] = std::vector<sf::Keyboard::Key>{button};
	PH_LOG(LogLevel::Info, "Action was added to ActionManager.");
}

void ActionManager::addKeyToAction(const std::string& action, sf::Keyboard::Key button)
{
	auto found = mActions.find(action);

	if(found != mActions.end()) {
		auto& vec = found->second;
		if(std::find(vec.begin(), vec.end(), button) == vec.end())
			found->second.emplace_back(button);
	}
	PH_LOG(LogLevel::Info, "Key was added to action.");
}

void ActionManager::deleteKeyFromAction(const std::string& action, sf::Keyboard::Key button)
{
	auto found = mActions.find(action);

	if(found != mActions.end()) {
		auto& vec = found->second;
		vec.erase(std::remove(vec.begin(), vec.end(), button), vec.end());
	}
	PH_LOG(LogLevel::Info, "Key was deleted from action.");
}

void ActionManager::deleteAction(const std::string& action)
{
	auto found = mActions.find(action);
	mActions.erase(found);
	PH_LOG(LogLevel::Info, "Action was deleted from ActionManager.");
}

void ActionManager::clearAllActions() noexcept
{
	mActions.clear();
	PH_LOG(LogLevel::Info, "All actions were cleared.");
}

bool ActionManager::isActionPressed(const std::string& action)
{
	if(!mEnabled)
		return false;

	for(const auto& button : mActions[action]) {
		if(sf::Keyboard::isKeyPressed(button))
			return true;
	}
	return false;
}

bool ActionManager::isActionJustPressed(const std::string& action)
{
	return isAction(action, EventLoop::isKeyJustPressed);
}

bool ActionManager::isActionJustReleased(const std::string& action)
{
	return isAction(action, EventLoop::isKeyJustReleased);
}

bool ActionManager::isAction(const std::string& action, std::function<bool(void)> func)
{
	if(!mEnabled)
		return false;

	for(const auto& button : mActions[action]) {
		if(func() && EventLoop::getKey() == button)
			return true;
	}
	return false;
}

}