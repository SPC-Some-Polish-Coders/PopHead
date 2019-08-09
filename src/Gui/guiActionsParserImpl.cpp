#include "guiActionsParserImpl.hpp"
#include "gui.hpp"
#include "Scenes/sceneManager.hpp"

namespace ph {

std::function<void(Widget*)> GuiActionsParserImpl::getGuiAction(GUI& gui, SceneManager& sceneManager, const std::string& actionStr) const
{
	auto pair = splitAction(actionStr);

	if (pair.first == "replaceScene")
		return [&sceneManager, pair](Widget*) { sceneManager.replaceScene(pair.second); };
}

std::pair<std::string, std::string> GuiActionsParserImpl::splitAction(const std::string& actionStr) const
{
	auto colonPos = actionStr.find(':');
	std::pair<std::string, std::string> splittedAction;
	splittedAction.first = actionStr.substr(0, colonPos);
	splittedAction.second = actionStr.substr(colonPos + 1);
	return splittedAction;
}
}
