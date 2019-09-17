#pragma once

#include "actionEvent.hpp"
#include <SFML/Window.hpp>
#include <variant>

namespace ph {

using Event = std::variant<sf::Event, ActionEvent>;

}