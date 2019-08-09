#pragma once

#include "widget.hpp"

#include <functional>
#include <string>

namespace ph {

class GUI;
class SceneManager;

class GuiActionsParser
{
public:
	virtual std::function<void(Widget*)> getGuiAction(GUI& gui, SceneManager& sceneManager, const std::string& actionStr) const = 0;
};
}
