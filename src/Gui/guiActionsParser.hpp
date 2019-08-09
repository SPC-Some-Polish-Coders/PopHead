#pragma once

#include "widget.hpp"

#include <functional>
#include <string>

namespace ph {

class GUI;
class SceneManager;
class GameCloser;

class GuiActionsParser
{
public:
	virtual std::function<void(Widget*)> getGuiAction(GUI&, SceneManager&, GameCloser&, const std::string& actionStr) const = 0;
};

}
