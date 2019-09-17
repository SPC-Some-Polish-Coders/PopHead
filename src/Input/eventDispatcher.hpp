#pragma once

#include "actionManager.hpp"
#include "event.hpp"
#include <variant>
#include <vector>
#include <SFML/Window.hpp>

namespace ph {

class EventDispatcher
{
public:
	static bool dispatchEvent(ph::Event&, sf::Window&);

private:
	inline static std::vector<ActionEvent> mPendingActionEvents;
};

}