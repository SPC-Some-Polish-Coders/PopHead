#include "xmlGuiParser.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"
#include "gui.hpp"
#include "Utilities/xml.hpp"

namespace ph {
namespace {
void throwMissingAttributeException(std::string attributeName) {
	throw std::runtime_error(attributeName + " is mandatory attribute");
}
}
std::unique_ptr<GuiActionsParser> XmlGuiParser::mActionsParser = nullptr;

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
	guiTag = guiTag.getChild("gui");

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
		widget.setColor(getColor(textWidgetTag)); // TODO_xml
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
	else throwMissingAttributeException("contentPathSlider");
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

sf::Color XmlGuiParser::getColor(const Xml& widgetTag)
{
	// TODO_xml: Use Xml::toColor()

	auto colorStr = widgetTag.getAttribute("color")->toString();

	if(colorStr.substr(0, 4) == "rgba")
		return parseRGBA(colorStr);
	else if(colorStr.substr(0, 3) == "rgb")
		return parseRGB(colorStr);
	else if(colorStr == "black")
		return sf::Color::Black;
	else if(colorStr == "white")
		return sf::Color::White;
	else if(colorStr == "red")
		return sf::Color::Red;
	else if(colorStr == "green")
		return sf::Color::Green;
	else if(colorStr == "blue")
		return sf::Color::Blue;
	else if(colorStr == "yellow")
		return sf::Color::Yellow;
	else if(colorStr == "magenta")
		return sf::Color::Magenta;
	else if(colorStr == "cyan")
		return sf::Color::Cyan;
	else if(colorStr == "transparent")
		return sf::Color::Transparent;
	else
		return sf::Color();
}

namespace {
sf::Uint8 intoUint8(const std::string& str)
{
	return static_cast<sf::Uint8>(std::stoul(str));
}
}

sf::Color XmlGuiParser::parseRGB(std::string colorStr)
{
	colorStr.pop_back();
	auto bracketPos = colorStr.find('(');

	std::vector<size_t> commas;
	commas.push_back(colorStr.find(','));
	commas.push_back(colorStr.find(',', commas[0] + 1));

	std::vector<sf::Uint8> values = {
		intoUint8(colorStr.substr(bracketPos + 1, commas[0] - bracketPos - 1)),
		intoUint8(colorStr.substr(commas[0] + 1, commas[1] - commas[0])),
		intoUint8(colorStr.substr(commas[1] + 1))
	};

	return sf::Color(values[0], values[1], values[2]);
}

sf::Color XmlGuiParser::parseRGBA(std::string colorStr)
{
	colorStr.pop_back();
	auto bracketPos = colorStr.find('(');

	std::vector<size_t> commas;
	commas.push_back(colorStr.find(','));
	commas.push_back(colorStr.find(',', commas[0] + 1));
	commas.push_back(colorStr.find(',', commas[1] + 1));

	std::vector<sf::Uint8> values = {
		intoUint8(colorStr.substr(bracketPos + 1, commas[0] - bracketPos - 1)),
		intoUint8(colorStr.substr(commas[0] + 1, commas[1] - commas[0])),
		intoUint8(colorStr.substr(commas[1] + 1, commas[2] - commas[1])),
		intoUint8(colorStr.substr(commas[2] + 1))
	};

	return sf::Color(values[0], values[1], values[2], values[3]);
}

}
