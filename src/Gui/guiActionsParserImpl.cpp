#include "guiActionsParserImpl.hpp"
#include "gui.hpp"
#include "Scenes/sceneManager.hpp"
#include "gameData.hpp"
#include "Logs/logs.hpp"

namespace ph {

std::function<void(Widget*)> GuiActionsParserImpl::getGuiAction(GUI& gui, SceneManager& sceneManager, GameCloser& gameCloser, const std::string& actionStr) const
{
	auto pair = getSplitAction(actionStr);

	if(pair.first == "replaceScene")
		return [&sceneManager, pair](Widget*) { sceneManager.replaceScene(pair.second); };
	else if(pair.first == "closeGame")
		return [&gameCloser](Widget*) {gameCloser.closeGame(); };
	else if(pair.first == "hideGuiInterface")
		return [&gui, pair](Widget*) {gui.hideInterface(pair.second); };
	else if(pair.first == "showGuiInterface")
		return [&gui, pair](Widget*) {gui.showInterface(pair.second); };

	PH_LOG_ERROR("Use of unsupported button action: " + actionStr);
	return [](Widget*) {};
}

std::pair<std::string, std::string> GuiActionsParserImpl::getSplitAction(const std::string& actionStr) const
{
	auto colonPos = actionStr.find(':');
	std::pair<std::string, std::string> splittedAction;
	splittedAction.first = actionStr.substr(0, colonPos);
	splittedAction.second = actionStr.substr(colonPos + 1);
	return splittedAction;
}

}
