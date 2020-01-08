#include "xmlGuiParser.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"
#include "gui.hpp"
#include "Utilities/xml.hpp"

namespace ph {

void XmlGuiParser::setActionsParser(std::unique_ptr<GuiActionsParser> actionsParser)
{
	mActionsParser = std::move(actionsParser);
}

void XmlGuiParser::parseFile(GameData* const gameData, const std::string& fileName)
{
	PH_LOG_INFO("Gui file (" + fileName + ") is being parsed.");

	mGameData = gameData;

	mGui = &(gameData->getGui());
	mFontHolder = &(gameData->getFonts());

	Xml guiTag;
	guiTag.loadFromFile(fileName);
	guiTag = *guiTag.getChild("gui");

	auto interfaces = guiTag.getChildren("interface");
	for(auto& interface : interfaces)
		parseInterface(interface, *mGui);
}

void XmlGuiParser::parseInterface(const Xml& interfaceTag, GUI& gui)
{
	auto interfaceName = interfaceTag.getAttribute("name")->toString();
	auto interface = gui.addInterface(interfaceName);
	handleInterfaceHideAttribute(interfaceName, interfaceTag, gui);
	parseWidgetAttributes(interfaceTag, *interface);
	parseWidgetChildren(interfaceTag, *interface);
}

void XmlGuiParser::handleInterfaceHideAttribute(const std::string& interfaceName, const Xml& interfaceTag, GUI& gui)
{
	if(auto hide = interfaceTag.getAttribute("hide"))
		if(hide->toBool())
			gui.hideInterface(interfaceName);
}

void XmlGuiParser::parseWidgetAttributes(const Xml& widgetTag, Widget& widget)
{
	if(auto contentPath = widgetTag.getAttribute("contentPath")) {
		const std::string path = contentPath->toString();
		mGui->getTextures().load(path);
		widget.setContentPath(path);
	}
	if(auto origin = widgetTag.getAttribute("origin"))
		widget.setOrigin(origin->toVector2f());
	if(auto position = widgetTag.getAttribute("position"))
		widget.setPosition(position->toVector2f());
	if(auto scale = widgetTag.getAttribute("scale"))
		widget.scale(scale->toVector2f());
	if(auto alpha = widgetTag.getAttribute("alpha"))
		widget.setAlpha(alpha->toUnsigned());
	if(auto hide = widgetTag.getAttribute("hide"))
		if(hide->toBool())
			widget.hide();

	if(mActionsParser) {
		if(auto onButtonPressed = widgetTag.getAttribute("onButtonPressed"))
			widget.addBehavior(BehaviorType::onPressed, mActionsParser->getGuiAction(*mGameData, onButtonPressed->toString()));
		if(auto onButtonReleased = widgetTag.getAttribute("onButtonReleased"))
			widget.addBehavior(BehaviorType::onReleased, mActionsParser->getGuiAction(*mGameData, onButtonReleased->toString()));
		if(auto onButtonUpdate = widgetTag.getAttribute("onButtonUpdate"))
			widget.addBehavior(BehaviorType::onUpdate, mActionsParser->getGuiAction(*mGameData, onButtonUpdate->toString()));
	}
}

void XmlGuiParser::parseTextWidgetAttributes(const Xml& textWidgetTag, TextWidget& widget)
{
	if(auto pathXml = textWidgetTag.getAttribute("fontPath")) {
		const std::string path = pathXml->toString();
		mFontHolder->load(path);
		widget.setFontPath(path);
	}
	if(auto textPosition = textWidgetTag.getAttribute("textPosition"))
		widget.setTextPosition(textPosition->toVector2f());
	if(auto color = textWidgetTag.getAttribute("color"))
		widget.setColor(color->toColor());
	if(auto text = textWidgetTag.getAttribute("text"))
		widget.setString(text->toString());
	if(auto characterSize = textWidgetTag.getAttribute("characterSize"))
		widget.setCharacterSize(characterSize->toUnsigned());
	if(auto textOrigin = textWidgetTag.getAttribute("textOrigin"))
		widget.setTextOrigin(textOrigin->toVector2f());
	if(auto textAlpha = textWidgetTag.getAttribute("textAlpha"))
		widget.setTextAlpha(textAlpha->toUnsigned());
	if(auto scaleText = textWidgetTag.getAttribute("scaleText"))
		widget.scaleText(scaleText->toVector2f());
	if(auto scrollingEffect = textWidgetTag.getAttribute("scrollingEffect"))
		widget.setScrollingEffect(scrollingEffect->toBool());
}

void XmlGuiParser::parseSliderWidgetAttributes(const Xml& widgetTag, SliderWidget& widget)
{
	if(auto contentPathSlider = widgetTag.getAttribute("contentPathSlider")) {
		const std::string path = contentPathSlider->toString();
		mGui->getTextures().load(path);
		widget.createSlider(path);
	}
	else
		PH_EXIT_GAME("GUI XML ERROR: contentPathSilder is mandatory attribute");
}

void XmlGuiParser::parseWidgetChildren(const Xml& widgetTag, Widget& widget)
{
	auto widgets = widgetTag.getChildren("widget");
	for(auto const& childTag : widgets) 
	{
		auto name = childTag.getAttribute("name")->toString();
		auto childWidget = new Widget;
		widget.addWidget(name, childWidget);
		parseWidgetAttributes(childTag, *childWidget);
		parseWidgetChildren(childTag, *childWidget);
	}

	auto sliderWidget = widgetTag.getChildren("sliderWidget");
	for(auto const& childTag : sliderWidget)
	{
		auto name = childTag.getAttribute("name")->toString();
		auto childWidget = new SliderWidget;
		widget.addWidget(name, childWidget);
		parseWidgetAttributes(childTag, *childWidget);
		parseSliderWidgetAttributes(childTag, *childWidget);
	}

	auto textWidgets = widgetTag.getChildren("textWidget");
	for(auto const& childTag : textWidgets)
	{
		auto name = childTag.getAttribute("name")->toString();
		auto childWidget = new TextWidget;
		widget.addWidget(name, childWidget);
		parseWidgetAttributes(childTag, *childWidget);
		parseTextWidgetAttributes(childTag, *childWidget);
		parseWidgetChildren(childTag, *childWidget);
	}
}

}

