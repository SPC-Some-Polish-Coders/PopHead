#pragma once

#include <variant>
#include <SFML/Window.hpp>

namespace ph {

class ActionEvent { int type; };

using Event = std::variant<sf::Event, ActionEvent>;

class EventDispatcher
{
public:
	static bool dispatchEvent(ph::Event&, sf::Window&);
};

}