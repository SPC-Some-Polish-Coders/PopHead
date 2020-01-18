#include "guiActionsParser.hpp"
#include "gui.hpp"
#include "Scenes/sceneManager.hpp"
#include "Utilities/cast.hpp"
#include "Logs/logs.hpp"
#include "sliderWidget.hpp"

namespace ph {

// TODO_gui: Make this parser work

std::function<void(Widget*)> GuiActionsParser::getGuiAction(const std::string& actionStr) const
{
	//auto& sceneManager = gameData.getSceneManager();
	//auto& gameCloser = gameData.getGameCloser();
	//auto& gui = gameData.getGui();
	//auto& musicPlayer = gameData.getMusicPlayer();
	//auto& soundPlayer = gameData.getSoundPlayer();


	//auto pair = getSplitAction(actionStr);

	//if (pair.first == "replaceScene")
	//	return [&sceneManager, pair](Widget*) { sceneManager.replaceScene(pair.second); };

	//else if (pair.first == "loadLastSave")
	//	return [&sceneManager, pair](Widget*) { sceneManager.replaceScene(sceneManager.getCurrentMapName()); };

	//else if (pair.first == "changeMusicVolume")
	//{
	//	return [&musicPlayer](Widget* widget)
	//	{
	//		auto volume = static_cast<SliderWidget*>(widget)->getSliderValue();
	//		musicPlayer.setVolume(static_cast<float>(volume));
	//	};
	//}
	//else if (pair.first == "changeSoundVolume")
	//{
	//	return [&soundPlayer](Widget* widget)
	//	{
	//		auto volume = static_cast<SliderWidget*>(widget)->getSliderValue();
	//		soundPlayer.setVolume(static_cast<float>(volume));
	//	};
	//}

	//else if(pair.first == "closeGame")
	//	return [&gameCloser](Widget*) {gameCloser.closeGame(); };

	//else if(pair.first == "hideGuiInterface")
	//	return [&gui, &pair](Widget*) {gui.hideInterface(pair.second.c_str()); };

	//else if(pair.first == "showGuiInterface")
	//	return [&gui, &pair](Widget*) {gui.showInterface(pair.second.c_str()); };

	//else if(pair.first == "setGamePause")
	//	return [&sceneManager, &gui, pair](Widget*) {
	//		bool pause = Cast::toBool(pair.second);
	//		sceneManager.getScene().setPause(pause);
	//		pause ? gui.showInterface("pauseScreen") : gui.hideInterface("pauseScreen");
	//	};

	PH_LOG_ERROR("Use of unsupported button action: " + actionStr);
	return [](Widget*) {};
}

std::pair<std::string, std::string> GuiActionsParser::getSplitAction(const std::string& actionStr) const
{
	auto colonPos = actionStr.find(':');
	std::pair<std::string, std::string> splittedAction;
	splittedAction.first = actionStr.substr(0, colonPos);
	splittedAction.second = actionStr.substr(colonPos + 1);
	return splittedAction;
}

}
