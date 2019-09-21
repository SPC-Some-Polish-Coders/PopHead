#pragma once

#include "actionEvent.hpp"
#include <SFML/Window.hpp>
#include <string>
#include <vector>
#include <map>

namespace ph {

class ActionEventManager
{
public:
	// TODO: Init it somewhere else
	static void init();

	static void addAction(const std::string& action, std::vector<sf::Keyboard::Key>);
	static void addAction(const std::string& action, sf::Keyboard::Key);
	static void addKeyToAction(const std::string& action, sf::Keyboard::Key);
	static void deleteKeyFromAction(const std::string& action, sf::Keyboard::Key);
	static void deleteAction(const std::string& action);
	static void clearAllActions() noexcept;

	static bool isEnabled() { return mEnabled; }
	static void setEnabled(bool enabled);

	static bool isActionPressed(const std::string& action);

	static void addActionEventsTo(std::vector<ActionEvent>&, const sf::Event currentSfmlEvent);

private:
	inline static std::map<std::string, std::vector<sf::Keyboard::Key>> mActions;
	inline static bool mEnabled;
};

}
