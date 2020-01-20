#include "xmlGuiParser.hpp"
#include "gui.hpp"
#include "Logs/logs.hpp"
#include "Utilities/xml.hpp"
#include "Scenes/sceneManager.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "GUI/gui.hpp"
#include "gameData.hpp"

namespace ph {

namespace {
	GUI* gui = nullptr;
	TextureHolder* textureHolder = nullptr;
	SceneManager* sceneManager = nullptr;
	GameCloser* gameCloser = nullptr;
	MusicPlayer* musicPlayer = nullptr;
	SoundPlayer* soundPlayer = nullptr;
}

void XmlGuiParser::init(GUI* g, TextureHolder* th, SceneManager* sm, GameCloser* gc, MusicPlayer* mp, SoundPlayer* sp)
{
	gui = g;
	textureHolder = th;
	sceneManager = sm;
	gameCloser = gc;
	musicPlayer = mp;
	soundPlayer = sp;
}

void XmlGuiParser::parseGuiXml(const std::string& filepath)
{
	PH_LOG_INFO("Gui file (" + filepath + ") is being parsed.");

	Xml guiNode;
	PH_ASSERT_CRITICAL(guiNode.loadFromFile(filepath), "Gui file \"" + filepath + "\" wasn't loaded correctly!");
	guiNode = *guiNode.getChild("gui");

	mWidgetTemplates = guiNode.getChildren("widgetTemplate");

	auto interfaceNodes = guiNode.getChildren("interface");
	for(auto& interfaceNode : interfaceNodes)
	{
		auto interfaceName = interfaceNode.getAttribute("name")->toString();
		auto interface = gui->addInterface(interfaceName.c_str());

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

	auto parseTemplateAttributes = [this](const Xml& widgetNode, Widget* widget, WidgetType widgetType)
	{
		if(auto templateName = widgetNode.getAttribute("template"))
			for(auto& widgetTemplate : mWidgetTemplates)
				if(auto name = widgetTemplate.getAttribute("name"))
					if(name->toString() == templateName->toString()) {
						parseWidgetAttributes(widgetTemplate, widget);
						if(widgetType == WidgetType::TextWidget)
							parseTextWidgetAttributes(widgetTemplate, dynamic_cast<TextWidget*>(widget));
						else if(widgetType == WidgetType::SliderWidget)
							parseSliderWidgetAttributes(widgetTemplate, dynamic_cast<SliderWidget*>(widget));
					}
	};

	auto widgets = widgetNode.getChildren("widget");
	for(auto const& childNode : widgets)
	{
		auto name = childNode.getAttribute("name")->toString();
		auto* childWidget = new Widget(name.c_str());
		widgetParent->addChildWidget(childWidget);
		parseTemplateAttributes(childNode, childWidget, WidgetType::Widget);
		parseWidgetAttributes(childNode, childWidget);
		parseChildren<Widget>(childNode, childWidget);
	}

	auto textWidgets = widgetNode.getChildren("textWidget");
	for(auto const& childNode : textWidgets)
	{
		auto name = childNode.getAttribute("name")->toString();
		auto* childWidget = new TextWidget(name.c_str());
		widgetParent->addChildWidget(childWidget);
		parseTemplateAttributes(childNode, childWidget, WidgetType::TextWidget);
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
		parseTemplateAttributes(childNode, childWidget, WidgetType::SliderWidget);
		parseWidgetAttributes(childNode, childWidget);
		parseSliderWidgetAttributes(childNode, childWidget);
		parseChildren<Widget>(childNode, childWidget);
	}
}

void XmlGuiParser::parseWidgetAttributes(const Xml& widgetNode, Widget* widget) const
{	
	if(auto texturePath = widgetNode.getAttribute("texturePath")) {
		const std::string path = texturePath->toString();
		if(textureHolder->load(path))
			widget->setTexture(&textureHolder->get(path));
		else
			PH_EXIT_GAME("XmlGuiParser error: Texture path wasn't properly loaded " + path);
	}
	if(auto size = widgetNode.getAttribute("size"))
		widget->setSize(size->toVector2f());
	if(auto pos = widgetNode.getAttribute("centerPosition"))
		widget->setCenterPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("leftCenterPosition"))
		widget->setLeftCenterPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("rightCenterPosition"))
		widget->setRightCenterPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("topCenterPosition"))
		widget->setTopCenterPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("bottomCenterPosition"))
		widget->setBottomCenterPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("topLeftPosition"))
		widget->setTopLeftPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("topRightPosition"))
		widget->setTopRightPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("bottomLeftPosition"))
		widget->setBottomLeftPosition(pos->toVector2f());
	if(auto pos = widgetNode.getAttribute("bottomRightPosition"))
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
	if(auto pathXml = textWidgetTag.getAttribute("fontName"))
		widget->setFontName(pathXml->toString().c_str());
	if(auto textColor = textWidgetTag.getAttribute("textColor"))
		widget->setTextColor(textColor->toColor());
	if(auto text = textWidgetTag.getAttribute("text"))
		widget->setText(text->toString());
	if(auto textSize = textWidgetTag.getAttribute("textSize"))
		widget->setTextSize(textSize->toFloat());
	if(auto scrollingEffect = textWidgetTag.getAttribute("scrollingEffect"))
		widget->setScrollingEffect(scrollingEffect->toBool());
}

void XmlGuiParser::parseSliderWidgetAttributes(const Xml& widgetTag, SliderWidget* widget) const
{
	if(auto iconTexturePath = widgetTag.getAttribute("iconTexturePath")) {
		const std::string path = iconTexturePath->toString();
		textureHolder->load(path);
		widget->setIconTexture(&textureHolder->get(path));
	}
	if(auto iconSize = widgetTag.getAttribute("iconSize"))
		widget->setIconSize(iconSize->toVector2f());
	if(auto sliderValue = widgetTag.getAttribute("sliderValue"))
		widget->setSliderValue(sliderValue->toFloat());
	if(auto maxSliderValue = widgetTag.getAttribute("maxSliderValue"))
		widget->setSliderMaxValue(maxSliderValue->toFloat());
}

std::function<void(Widget*)> XmlGuiParser::getGuiAction(const std::string& actionStr) const
{
	auto colonPos = actionStr.find(':');
	auto name = actionStr.substr(0, colonPos);
	auto data = actionStr.substr(colonPos + 1);

	if(name == "replaceScene") {
		return [&data](Widget*) { sceneManager->replaceScene(data); };
	}
	else if(name == "loadLastSave") {
		return [](Widget*) { sceneManager->replaceScene(sceneManager->getCurrentMapName()); };
	}
	else if(name == "changeMusicVolume") {
		return [](Widget* widget) {
			auto volume = static_cast<SliderWidget*>(widget)->getSliderValue();
			musicPlayer->setVolume(static_cast<float>(volume));
		};
	}
	else if(name == "changeSoundVolume") {
		return [](Widget* widget) {
			auto volume = static_cast<SliderWidget*>(widget)->getSliderValue();
			soundPlayer->setVolume(static_cast<float>(volume));
		};
	}
	else if(name == "closeGame") {
		return [](Widget*) {gameCloser->closeGame(); };
	}
	else if(name == "hideGuiInterface") {
		return [&data](Widget*) {gui->hideInterface(data.c_str()); };
	}
	else if(name == "showGuiInterface") {
		return [&data](Widget*) {gui->showInterface(data.c_str()); };
	}
	else if(name == "setGamePause") {
		return [&data](Widget*) {
			bool pause = Cast::toBool(data);
			sceneManager->getScene().setPause(pause);
			pause ? gui->showInterface("pauseScreen") : gui->hideInterface("pauseScreen");
		};
	}

	PH_LOG_ERROR("Use of unsupported button action: " + actionStr);
	return [](Widget*) {};
}

}

