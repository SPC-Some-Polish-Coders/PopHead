#include "xmlGuiParser.hpp"
#include "gui.hpp"
#include "Logs/logs.hpp"
#include "Utilities/xml.hpp"

namespace ph {

static void parseChildren(const Xml& widgetTag, WidgetParent* widget, TextureHolder* textureHolder);
static void parseWidgetAttributes(const Xml& widgetTag, Widget* widget, TextureHolder* textureHolder);
static void parseTextWidgetAttributes(const Xml& textWidgetTag, TextWidget* widget);
static void parseSliderWidgetAttributes(const Xml& widgetTag, SliderWidget* widget, TextureHolder* textureHolder);

void parseGuiXml(const std::string& fileName, GUI* gui, TextureHolder* textureHolder)
{
	PH_LOG_INFO("Gui file (" + fileName + ") is being parsed.");

	Xml guiTag;
	PH_ASSERT_CRITICAL(guiTag.loadFromFile(fileName), "Gui file \"" + fileName + "\" wasn't loaded correctly!");
	guiTag = *guiTag.getChild("gui");

	auto interfaceNodes = guiTag.getChildren("interface");
	for(auto& interfaceNode : interfaceNodes)
	{
		auto interfaceName = interfaceNode.getAttribute("name")->toString();
		auto interface = gui->addInterface(interfaceName.c_str());

		if(auto hide = interfaceNode.getAttribute("hide"))
			if(hide->toBool())
				interface->hide();

		parseChildren(interfaceNode, interface, textureHolder);
	}
}

static void parseChildren(const Xml& widgetNode, WidgetParent* widgetParent, TextureHolder* textureHolder)
{
	auto widgets = widgetNode.getChildren("widget");
	for(auto const& childTag : widgets) 
	{
		auto name = childTag.getAttribute("name")->toString();
		auto* childWidget = new Widget(name.c_str());
		widgetParent->addChildWidget(childWidget);
		parseWidgetAttributes(childTag, childWidget, textureHolder);
		parseChildren(childTag, childWidget, textureHolder);
	}

	auto textWidgets = widgetNode.getChildren("textWidget");
	for(auto const& childTag : textWidgets)
	{
		auto name = childTag.getAttribute("name")->toString();
		auto* childWidget = new TextWidget(name.c_str());
		widgetParent->addChildWidget(childWidget);
		parseWidgetAttributes(childTag, childWidget, textureHolder);
		parseTextWidgetAttributes(childTag, childWidget);
		parseChildren(childTag, childWidget, textureHolder);
	}

	auto sliderWidget = widgetNode.getChildren("sliderWidget");
	for(auto const& childTag : sliderWidget)
	{
		auto name = childTag.getAttribute("name")->toString();
		auto childWidget = new SliderWidget(name.c_str());
		widgetParent->addChildWidget(childWidget);
		parseWidgetAttributes(childTag, childWidget, textureHolder);
		parseSliderWidgetAttributes(childTag, childWidget, textureHolder);
		parseChildren(childTag, childWidget, textureHolder);
	}
}

void parseWidgetAttributes(const Xml& widgetTag, Widget* widget, TextureHolder* textureHolder)
{
	if(auto texturePath = widgetTag.getAttribute("texturePath")) {
		const std::string path = texturePath->toString();
		if(textureHolder->load(path))
			widget->setTexture(&textureHolder->get(path));
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

	// TODO_gui
	//if(mActionsParser) {
	//	if(auto onButtonPressed = widgetTag.getAttribute("onButtonPressed"))
	//		widget.addBehavior(BehaviorType::onPressed, mActionsParser->getGuiAction(*mGameData, onButtonPressed->toString()));
	//	if(auto onButtonReleased = widgetTag.getAttribute("onButtonReleased"))
	//		widget.addBehavior(BehaviorType::onReleased, mActionsParser->getGuiAction(*mGameData, onButtonReleased->toString()));
	//	if(auto onButtonUpdate = widgetTag.getAttribute("onButtonUpdate"))
	//		widget.addBehavior(BehaviorType::onUpdate, mActionsParser->getGuiAction(*mGameData, onButtonUpdate->toString()));
	//}
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

static void parseSliderWidgetAttributes(const Xml& widgetTag, SliderWidget* widget, TextureHolder* textureHolder)
{
	if(auto iconTexturePath = widgetTag.getAttribute("iconTexturePath")) {
		const std::string path = iconTexturePath->toString();
		textureHolder->load(path);
		widget->setIconTexture(&textureHolder->get(path));
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

}

