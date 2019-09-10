#include "guiActionsParserImpl.hpp"
#include "gui.hpp"
#include "Scenes/sceneManager.hpp"
#include "Utilities/cast.hpp"
#include "gameData.hpp"
#include "Logs/logs.hpp"

namespace ph {

std::function<void(Widget*)> GuiActionsParserImpl::getGuiAction(GUI& gui, SceneManager& sceneManager, GameCloser& gameCloser, const std::string& actionStr) const
{
	auto pair = getSplitAction(actionStr);

	if (pair.first == "replaceScene")
		return [&sceneManager, pair](Widget*) { sceneManager.replaceScene(pair.second); };

	else if (pair.first == "loadLastSave")
		return [&sceneManager, pair](Widget*) { sceneManager.replaceScene(sceneManager.getCurrentMapName()); };

	else if(pair.first == "closeGame")
		return [&gameCloser](Widget*) {gameCloser.closeGame(); };

	else if(pair.first == "hideGuiInterface")
		return [&gui, pair](Widget*) {gui.hideInterface(pair.second); };

	else if(pair.first == "showGuiInterface")
		return [&gui, pair](Widget*) {gui.showInterface(pair.second); };

	else if(pair.first == "setGamePause")
		return [&sceneManager, &gui, pair](Widget*) {
			bool pause = Cast::toBool(pair.second);
			sceneManager.getScene().setPause(pause);
			pause ? gui.showInterface("pauseScreen") : gui.hideInterface("pauseScreen");
		};

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
