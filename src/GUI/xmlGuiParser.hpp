#pragma once

#include "guiActionsParser.hpp"
#include "sliderWidget.hpp"
#include "Resources/resourceHolder.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <memory>
#include <string>

namespace ph {

class GameData;
class Xml;
class GUI;
class Widget;
class TextWidget;
class SliderWidget;

class XmlGuiParser
{
public:
	static void setActionsParser(std::unique_ptr<GuiActionsParser> actionsParser);

	void parseFile(GameData* const gameData, const std::string& fileName);
	void parseInterface(const Xml& interfaceTag, GUI& gui);
	void handleInterfaceHideAttribute(const char* interfaceName, const Xml& interfaceTag, GUI& gui);
	void parseWidgetAttributes(const Xml& widgetTag, Widget& widget);
	void parseTextWidgetAttributes(const Xml& textWidgetTag, TextWidget& widget);
	void parseSliderWidgetAttributes(const Xml& textWidgetTag, SliderWidget& widget);
	void parseWidgetChildren(const Xml& widgetTag, Widget& widget);

private:
	GameData* mGameData;
	GUI* mGui;
	OldFontHolder* mFontHolder;
	inline static std::unique_ptr<GuiActionsParser> mActionsParser = nullptr;
};

}
