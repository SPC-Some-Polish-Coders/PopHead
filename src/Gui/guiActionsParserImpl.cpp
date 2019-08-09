#include "guiActionsParserImpl.hpp"
#include "gui.hpp"
#include "Scenes/sceneManager.hpp"
#include "gameData.hpp"
#include "Logs/logs.hpp"

namespace ph {

std::function<void(Widget*)> GuiActionsParserImpl::getGuiAction(GUI& gui, SceneManager& sceneManager, GameCloser& gameCloser, const std::string& actionStr) const
{
	auto pair = splitAction(actionStr);

	if(pair.first == "replaceScene")
		return [&sceneManager, pair](Widget*) { sceneManager.replaceScene(pair.second); };
	else if(pair.first == "closeGame")
		return [&gameCloser](Widget*) {gameCloser.closeGame(); };
	PH_LOG_ERROR("Use of unsupported button action: " + actionStr);
	return [](Widget*) {};
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
