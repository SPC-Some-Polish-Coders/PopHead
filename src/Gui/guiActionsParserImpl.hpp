#pragma once

#include "guiActionsParser.hpp"

namespace ph {


class GuiActionsParserImpl : public GuiActionsParser
{
public:
	std::function<void(Widget*)> getGuiAction(GUI& gui, SceneManager& sceneManager, const std::string& actionStr) const override;

private:
	std::pair<std::string, std::string> splitAction(const std::string& actionStr) const;
};
}
