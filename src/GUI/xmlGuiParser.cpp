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

static void parseChildren(const Xml& widgetTag, WidgetParent* widget, TextureHolder& textureHolder, SceneManager& sceneManager,
                          GameCloser& gameCloser, GUI& gui, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer);

static void parseWidgetAttributes(const Xml& widgetTag, Widget* widget, TextureHolder& textureHolder, SceneManager& sceneManager,
                          GameCloser& gameCloser, GUI& gui, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer);

static void parseTextWidgetAttributes(const Xml& textWidgetTag, TextWidget* widget);

static void parseSliderWidgetAttributes(const Xml& widgetTag, SliderWidget* widget, TextureHolder& textureHolder);

static std::function<void(Widget*)> getGuiAction(const std::string& actionStr, SceneManager& sceneManager, GameCloser& gameCloser,
                                                 GUI& gui, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer);


void parseGuiXml(const std::string& fileName, GUI& gui, TextureHolder& textureHolder, SceneManager& sceneManager,
                 GameCloser& gameCloser, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer)
{
	PH_LOG_INFO("Gui file (" + fileName + ") is being parsed.");

	Xml guiTag;
	PH_ASSERT_CRITICAL(guiTag.loadFromFile(fileName), "Gui file \"" + fileName + "\" wasn't loaded correctly!");
	guiTag = *guiTag.getChild("gui");

	auto interfaceNodes = guiTag.getChildren("interface");
	for(auto& interfaceNode : interfaceNodes)
	{
		auto interfaceName = interfaceNode.getAttribute("name")->toString();
		auto interface = gui.addInterface(interfaceName.c_str());

		if(auto hide = interfaceNode.getAttribute("hide"))
			if(hide->toBool())
				interface->hide();

		parseChildren(interfaceNode, interface, textureHolder, sceneManager, gameCloser, gui, musicPlayer, soundPlayer);
	}
}

static void parseChildren(const Xml& widgetNode, WidgetParent* widgetParent, TextureHolder& textureHolder, SceneManager& sceneManager,
                          GameCloser& gameCloser, GUI& gui, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer)
{
	auto widgets = widgetNode.getChildren("widget");
	for(auto const& childTag : widgets)
	{
		auto name = childTag.getAttribute("name")->toString();
		auto* childWidget = new Widget(name.c_str());
		widgetParent->addChildWidget(childWidget);
		parseWidgetAttributes(childTag, childWidget, textureHolder, sceneManager, gameCloser, gui, musicPlayer, soundPlayer);
		parseChildren(childTag, childWidget, textureHolder, sceneManager, gameCloser, gui, musicPlayer, soundPlayer);
	}

	auto textWidgets = widgetNode.getChildren("textWidget");
	for(auto const& childTag : textWidgets)
	{
		auto name = childTag.getAttribute("name")->toString();
		auto* childWidget = new TextWidget(name.c_str());
		widgetParent->addChildWidget(childWidget);
		parseWidgetAttributes(childTag, childWidget, textureHolder, sceneManager, gameCloser, gui, musicPlayer, soundPlayer);
		parseTextWidgetAttributes(childTag, childWidget);
		parseChildren(childTag, childWidget, textureHolder, sceneManager, gameCloser, gui, musicPlayer, soundPlayer);
	}

	auto sliderWidget = widgetNode.getChildren("sliderWidget");
	for(auto const& childTag : sliderWidget)
	{
		auto name = childTag.getAttribute("name")->toString();
		auto childWidget = new SliderWidget(name.c_str());
		widgetParent->addChildWidget(childWidget);
		parseWidgetAttributes(childTag, childWidget, textureHolder, sceneManager, gameCloser, gui, musicPlayer, soundPlayer);
		parseSliderWidgetAttributes(childTag, childWidget, textureHolder);
		parseChildren(childTag, childWidget, textureHolder, sceneManager, gameCloser, gui, musicPlayer, soundPlayer);
	}
}

void parseWidgetAttributes(const Xml& widgetTag, Widget* widget, TextureHolder& textureHolder, SceneManager& sceneManager,
                           GameCloser& gameCloser, GUI& gui, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer)
{
	if(auto texturePath = widgetTag.getAttribute("texturePath")) {
		const std::string path = texturePath->toString();
		if(textureHolder.load(path))
			widget->setTexture(&textureHolder.get(path));
		else
			PH_EXIT_GAME("XmlGuiParser error: Texture path wasn't properly loaded " + path);
	}
	if(auto size = widgetTag.getAttribute("size"))
		widget->setSize(size->toVector2f());
	if(auto pos = widgetTag.getAttribute("centerPosition"))
		widget->setCenterPosition(pos->toVector2f());
	if(auto pos = widgetTag.getAttribute("leftCenterPosition"))
		widget->setLeftCenterPosition(pos->toVector2f());
	if(auto pos = widgetTag.getAttribute("rightCenterPosition"))
		widget->setRightCenterPosition(pos->toVector2f());
	if(auto pos = widgetTag.getAttribute("topCenterPosition"))
		widget->setTopCenterPosition(pos->toVector2f());
	if(auto pos = widgetTag.getAttribute("bottomCenterPosition"))
		widget->setBottomCenterPosition(pos->toVector2f());
	if(auto pos = widgetTag.getAttribute("topLeftPosition"))
		widget->setTopLeftPosition(pos->toVector2f());
	if(auto pos = widgetTag.getAttribute("topRightPosition"))
		widget->setTopRightPosition(pos->toVector2f());
	if(auto pos = widgetTag.getAttribute("bottomLeftPosition"))
		widget->setBottomLeftPosition(pos->toVector2f());
	if(auto pos = widgetTag.getAttribute("bottomRightPosition"))
		widget->setBottomRightPosition(pos->toVector2f());
	if(auto color = widgetTag.getAttribute("color"))
		widget->setColor(color->toColor());
	if(auto hide = widgetTag.getAttribute("hide"))
		if(hide->toBool())
			widget->hide();

	if(auto onButtonPressed = widgetTag.getAttribute("onButtonPressed"))
		widget->addBehavior(BehaviorType::onPressed, getGuiAction(onButtonPressed->toString(), sceneManager, gameCloser, gui, musicPlayer, soundPlayer));
	if(auto onButtonReleased = widgetTag.getAttribute("onButtonReleased"))
		widget->addBehavior(BehaviorType::onReleased, getGuiAction(onButtonReleased->toString(), sceneManager, gameCloser, gui, musicPlayer, soundPlayer));
	if(auto onButtonUpdate = widgetTag.getAttribute("onButtonUpdate"))
		widget->addBehavior(BehaviorType::onUpdate, getGuiAction(onButtonUpdate->toString(), sceneManager, gameCloser, gui, musicPlayer, soundPlayer));
}

static void parseTextWidgetAttributes(const Xml& textWidgetTag, TextWidget* widget)
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

static void parseSliderWidgetAttributes(const Xml& widgetTag, SliderWidget* widget, TextureHolder& textureHolder)
{
	if(auto iconTexturePath = widgetTag.getAttribute("iconTexturePath")) {
		const std::string path = iconTexturePath->toString();
		textureHolder.load(path);
		widget->setIconTexture(&textureHolder.get(path));
	}
	else
		PH_EXIT_GAME("GUI XML ERROR: iconTexturePath is mandatory attribute");
	if(auto iconSize = widgetTag.getAttribute("iconSize"))
		widget->setIconSize(iconSize->toVector2f());
	if(auto sliderValue = widgetTag.getAttribute("sliderValue"))
		widget->setSliderValue(sliderValue->toFloat());
	if(auto maxSliderValue = widgetTag.getAttribute("maxSliderValue"))
		widget->setSliderMaxValue(maxSliderValue->toFloat());
}

static std::function<void(Widget*)> getGuiAction(const std::string& actionStr, SceneManager& sceneManager, GameCloser& gameCloser,
                                                 GUI& gui, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer)
{
	auto colonPos = actionStr.find(':');
	auto name = actionStr.substr(0, colonPos);
	auto data = actionStr.substr(colonPos + 1);

	if(name == "replaceScene") {
		return [&sceneManager, &data](Widget*) { sceneManager.replaceScene(data); };
	}
	else if(name == "loadLastSave") {
		return [&sceneManager](Widget*) { sceneManager.replaceScene(sceneManager.getCurrentMapName()); };
	}
	else if(name == "changeMusicVolume") {
		return [&musicPlayer](Widget* widget) {
			auto volume = static_cast<SliderWidget*>(widget)->getSliderValue();
			musicPlayer.setVolume(static_cast<float>(volume));
		};
	}
	else if(name == "changeSoundVolume") {
		return [&soundPlayer](Widget* widget) {
			auto volume = static_cast<SliderWidget*>(widget)->getSliderValue();
			soundPlayer.setVolume(static_cast<float>(volume));
		};
	}
	else if(name == "closeGame") {
		return [&gameCloser](Widget*) {gameCloser.closeGame(); };
	}
	else if(name == "hideGuiInterface") {
		return [&gui, &data](Widget*) {gui.hideInterface(data.c_str()); };
	}
	else if(name == "showGuiInterface") {
		return [&gui, &data](Widget*) {gui.showInterface(data.c_str()); };
	}
	else if(name == "setGamePause") {
		return [&sceneManager, &gui, &data](Widget*) {
			bool pause = Cast::toBool(data);
			sceneManager.getScene().setPause(pause);
			pause ? gui.showInterface("pauseScreen") : gui.hideInterface("pauseScreen");
		};
	}

	PH_LOG_ERROR("Use of unsupported button action: " + actionStr);
	return [](Widget*) {};
}

}

