#pragma once

#include <SFML/Window.hpp>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace ph {

class ActionManager
{
public:
	ActionManager();

	void addAction(const std::string& action, std::vector<sf::Keyboard::Key>);
	void addAction(const std::string& action, sf::Keyboard::Key);
	void addKeyToAction(const std::string& action, sf::Keyboard::Key);
	void deleteKeyFromAction(const std::string& action, sf::Keyboard::Key);
	void deleteAction(const std::string& action);
	void clearAllActions() noexcept;

	void setEnabled(bool enabled) { mEnabled = enabled; }

	bool isActionPressed(const std::string& action);
	bool isActionJustPressed(const std::string& action);
	bool isActionJustReleased(const std::string& action);
private:
	bool isAction(const std::string& action, std::function<bool(void)> func);

private:
	std::map< std::string, std::vector<sf::Keyboard::Key> > mActions;
	bool mEnabled;
};

}
