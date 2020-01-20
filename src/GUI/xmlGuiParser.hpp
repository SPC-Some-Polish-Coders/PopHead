#pragma once

#include "Resources/resourceHolder.hpp"
#include "Utilities/xml.hpp"
#include <string>
#include <vector>
#include <functional>

namespace ph {

class GUI;
class SceneManager;
class GameCloser;
class MusicPlayer;
class SoundPlayer;
class WidgetParent;		
class Widget;
class TextWidget;
class SliderWidget;

class XmlGuiParser
{
public:
	static void init(GUI* gui, TextureHolder* th, SceneManager* sm, GameCloser* gc, MusicPlayer* mp, SoundPlayer* sp);

	void parseGuiXml(const std::string& filepath);
private:
	void parseChildren(const Xml& widgetNode, WidgetParent* widgetParent) const;
	void parseWidgetAttributes(const Xml& widgetNode, Widget* widget) const;
	void parseTextWidgetAttributes(const Xml& textWidgetTag, TextWidget* widget) const;
	void parseSliderWidgetAttributes(const Xml& widgetTag, SliderWidget* widget) const;
	std::function<void(Widget*)> getGuiAction(const std::string& actionStr) const;

private:
	std::vector<Xml> mWidgetTemplates;
};

}

