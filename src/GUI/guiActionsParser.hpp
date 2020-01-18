#pragma once

#include "widget.hpp"
#include <functional>
#include <string>

namespace ph {

class GuiActionsParser
{
public:
	std::function<void(Widget*)> getGuiAction(const std::string& actionStr) const;
private:
	std::pair<std::string, std::string> getSplitAction(const std::string& actionStr) const;
};

}

