#include "xmlGuiParser.hpp"
#include "gui.hpp"
#include "game.hpp"
#include "Logs/logs.hpp"
#include "Utilities/xml.hpp"
#include "Scenes/sceneManager.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "GUI/gui.hpp"
#include <functional>
#include <string_view>

namespace ph {

namespace {
	TextureHolder* textureHolder = nullptr;
	SceneManager* sceneManager = nullptr;
}

void XmlGuiParser::init(TextureHolder* th, SceneManager* sm)
{
	textureHolder = th;
	sceneManager = sm;
}

void XmlGuiParser::parseGuiXml(const std::string& filepath)
{
	PH_LOG_INFO("Gui file (" + filepath + ") is being parsed.");

	GUI::clear();

	Xml guiNode;
	PH_ASSERT_CRITICAL(guiNode.loadFromFile(filepath), "Gui file \"" + filepath + "\" wasn't loaded correctly!");
	guiNode = *guiNode.getChild("gui");

	mWidgetTemplates = guiNode.getChildren("widgetTemplate");

	auto interfaceNodes = guiNode.getChildren("interface");
	for(auto& interfaceNode : interfaceNodes)
	{
		auto interfaceName = interfaceNode.getAttribute("name")->toString();
		auto interface = GUI::addInterface(interfaceName.c_str());

		if(auto hide = interfaceNode.getAttribute("hide"))
			if(hide->toBool())
				interface->hide();

		parseChildren<Interface>(interfaceNode, interface);
	}
}

template<typename WidgetParent>
void XmlGuiParser::parseChildren(const Xml& widgetNode, WidgetParent* widgetParent) const
{
	enum class WidgetType {Widget, TextWidget, SliderWidget};

	std::function<void(const Xml*, Widget*, WidgetType)> parseTemplateAttributes;
	parseTemplateAttributes = [this, &parseTemplateAttributes](const Xml* widgetNode, Widget* widget, WidgetType widgetType)
	{
		if(auto templateName = widgetNode->getAttribute("template"))
			for(auto& widgetTemplate : mWidgetTemplates)
				if(auto name = widgetTemplate.getAttribute("name"))
					if(name->toString() == templateName->toString()) 
					{
						parseTemplateAttributes(&widgetTemplate, widget, WidgetType::Widget);
						parseWidgetAttributes(widgetTemplate, widget);
						if(widgetType == WidgetType::TextWidget) {
							parseTemplateAttributes(&widgetTemplate, widget, WidgetType::TextWidget);
							parseTextWidgetAttributes(widgetTemplate, dynamic_cast<TextWidget*>(widget));
						}
						else if(widgetType == WidgetType::SliderWidget) {
							parseTemplateAttributes(&widgetTemplate, widget, WidgetType::SliderWidget);
							parseSliderWidgetAttributes(widgetTemplate, dynamic_cast<SliderWidget*>(widget));
						}
					}
	};

	auto widgets = widgetNode.getChildren("widget");
	for(auto const& childNode : widgets)
	{
		auto name = childNode.getAttribute("name")->toString();
		auto* childWidget = new Widget(name.c_str());
		widgetParent->addChildWidget(childWidget);
		parseTemplateAttributes(&childNode, childWidget, WidgetType::Widget);
		parseWidgetAttributes(childNode, childWidget);
		parseChildren<Widget>(childNode, childWidget);
	}

	auto textWidgets = widgetNode.getChildren("textWidget");
	for(auto const& childNode : textWidgets)
	{
		auto name = childNode.getAttribute("name")->toString();
		auto* childWidget = new TextWidget(name.c_str());
		widgetParent->addChildWidget(childWidget);
		parseTemplateAttributes(&childNode, childWidget, WidgetType::TextWidget);
		parseWidgetAttributes(childNode, childWidget);
		parseTextWidgetAttributes(childNode, childWidget);
		parseChildren<Widget>(childNode, childWidget);
	}

	auto sliderWidget = widgetNode.getChildren("sliderWidget");
	for(auto const& childNode : sliderWidget)
	{
		auto name = childNode.getAttribute("name")->toString();
		auto childWidget = new SliderWidget(name.c_str());
		widgetParent->addChildWidget(childWidget);
		parseTemplateAttributes(&childNode, childWidget, WidgetType::SliderWidget);
		parseWidgetAttributes(childNode, childWidget);
		parseSliderWidgetAttributes(childNode, childWidget);
		parseChildren<Widget>(childNode, childWidget);
	}
}

void XmlGuiParser::parseWidgetAttributes(const Xml& widgetNode, Widget* widget) const
{	
	if(auto texturePath = widgetNode.getAttribute("texturePath")) {
		const std::string path = "textures/" + texturePath->toString();
		PH_ASSERT_CRITICAL(textureHolder->load(path), "XmlGuiParser error: Texture path wasn't properly loaded " + path)
		widget->setTexture(&textureHolder->get(path));
	}
	if(auto size = widgetNode.getAttribute("size"))
		widget->setSize(size->toVector2f());
	if(auto vel = widgetNode.getAttribute("velocity"))
		widget->setVelocity(vel->toVector2f());
	if(auto pos = widgetNode.getAttribute("centerPos"))
		widget->setCenterPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("leftCenterPos"))
		widget->setLeftCenterPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("rightCenterPos"))
		widget->setRightCenterPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("topCenterPos"))
		widget->setTopCenterPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("bottomCenterPos"))
		widget->setBottomCenterPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("topLeftPos"))
		widget->setTopLeftPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("topRightPos"))
		widget->setTopRightPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("bottomLeftPos"))
		widget->setBottomLeftPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("bottomRightPos"))
		widget->setBottomRightPosition(pos->toVector2f());
	if(auto color = widgetNode.getAttribute("color"))
		widget->setColor(color->toColor());
	if(auto hide = widgetNode.getAttribute("hide"))
		if(hide->toBool())
			widget->hide();

	if(auto onButtonPressed = widgetNode.getAttribute("onButtonPressed"))
		widget->addBehavior(BehaviorType::onPressed, getGuiAction(onButtonPressed->toString()));
	if(auto onButtonReleased = widgetNode.getAttribute("onButtonReleased"))
		widget->addBehavior(BehaviorType::onReleased, getGuiAction(onButtonReleased->toString()));
	if(auto onButtonUpdate = widgetNode.getAttribute("onButtonUpdate"))
		widget->addBehavior(BehaviorType::onUpdate, getGuiAction(onButtonUpdate->toString()));
}

void XmlGuiParser::parseTextWidgetAttributes(const Xml& textWidgetTag, TextWidget* widget) const
{
	if(auto aligment = textWidgetTag.getAttribute("textAligment")) {
		const std::string aligmentStr = aligment->toString();
		if(aligmentStr == "center")
			widget->setTextAligment(TextAligment::center);
		if(aligmentStr == "left")
			widget->setTextAligment(TextAligment::left);
		if(aligmentStr == "right")
			widget->setTextAligment(TextAligment::right);
	}
	if(auto pathXml = textWidgetTag.getAttribute("fontName"))
		widget->setFontName(pathXml->toString().c_str());
	if(auto textColor = textWidgetTag.getAttribute("textColor"))
		widget->setTextColor(textColor->toColor());
	if(auto text = textWidgetTag.getAttribute("text"))
		widget->setText(text->toString());
	if(auto fontSize = textWidgetTag.getAttribute("fontSize"))
		widget->setFontSize(fontSize->toFloat());
}

void XmlGuiParser::parseSliderWidgetAttributes(const Xml& widgetTag, SliderWidget* widget) const
{
	if(auto iconTexturePath = widgetTag.getAttribute("iconTexturePath")) {
		const std::string path = "textures/" + iconTexturePath->toString();
		PH_ASSERT_CRITICAL(textureHolder->load(path), "XmlGuiParser error: Icon texture path wasn't properly loaded " + path)
		widget->setIconTexture(&textureHolder->get(path));
	}
	if(auto iconSize = widgetTag.getAttribute("iconSize"))
		widget->setIconSize(iconSize->toVector2f());

	auto getSliderValue = [](const Xml& sliderValueNode)
	{
		const std::string sliderValueStr = sliderValueNode.toString();
		if(sliderValueStr.find("get:") != std::string::npos) {
			std::string_view varName(sliderValueStr);
			varName.remove_prefix(4);
			if(varName == "SoundVolume")
				return SoundPlayer::getVolume();
			if(varName == "MusicVolume")
				return MusicPlayer::getVolume();
			PH_EXIT_GAME("XmlGuiParser error: You're trying to get unknown variable \"get:" + std::string(varName.data()));
		}
		else {
			return sliderValueNode.toFloat();
		}
	};
	if(auto initValue = widgetTag.getAttribute("initValue"))
		widget->setSliderValue(getSliderValue(*initValue));
	if(auto minValue = widgetTag.getAttribute("minValue"))
		widget->setSliderMinValue(getSliderValue(*minValue));
	if(auto maxValue = widgetTag.getAttribute("maxValue"))
		widget->setSliderMaxValue(getSliderValue(*maxValue));
}

std::function<void(Widget*)> XmlGuiParser::getGuiAction(const std::string& actionStr) const
{
	auto colonPos = actionStr.find(':');
	auto name = actionStr.substr(0, colonPos);
	auto data = actionStr.substr(colonPos + 1);

	if(name == "replaceScene") {
		return [data](Widget*) { sceneManager->replaceScene("scenes/" + data); };
	}
	else if(name == "loadLastSave") {
		return [](Widget*) { sceneManager->replaceScene(sceneManager->getCurrentSceneFilePath()); };
	}
	else if(name == "set") {
		if(data == "MusicVolume")
			return [](Widget* widget) { MusicPlayer::setVolume(static_cast<SliderWidget*>(widget)->getSliderValue()); };
		if(data == "SoundVolume")
			return [](Widget* widget) { SoundPlayer::setVolume(static_cast<SliderWidget*>(widget)->getSliderValue()); };
	}
	else if(name == "closeGame") {
		return [](Widget*) { Game::close(); };
	}
	else if(name == "hideGuiInterface") {
		return [data](Widget*) {GUI::hideInterface(data.c_str()); };
	}
	else if(name == "showGuiInterface") {
		return [data](Widget*) {GUI::showInterface(data.c_str()); };
	}
	else if(name == "setGamePause") {
		return [data](Widget*) {
			bool pause = Cast::toBool(data);
			sceneManager->getScene().setPause(pause);
			pause ? GUI::showInterface("pauseScreen") : GUI::hideInterface("pauseScreen");
		};
	}

	PH_LOG_ERROR("Use of unsupported button action: " + actionStr);
	return [](Widget*) {};
}

}

