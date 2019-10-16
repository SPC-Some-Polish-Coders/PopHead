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

	auto& gui = gameData->getGui();

	Xml guiTag;
	guiTag.loadFromFile(fileName);
	guiTag = guiTag.getChild("gui");

	auto interfaces = guiTag.getChildren("interface");
	for (auto& interface : interfaces)
		parseInterface(interface, gui);
}

void XmlGuiParser::parseInterface(const Xml& interfaceTag, GUI& gui)
{
	auto interfaceName = interfaceTag.getAttribute("name").toString();
	auto interface = gui.addInterface(interfaceName);
	handleInterfaceHideAttribute(interfaceName, interfaceTag, gui);
	parseWidgetAttributes(interfaceTag, *interface);
	parseWidgetChildren(interfaceTag, *interface);
}

void XmlGuiParser::handleInterfaceHideAttribute(const std::string& interfaceName, const Xml& interfaceTag, GUI& gui)
{
	if(interfaceTag.hasAttribute("hide")) {
		bool hideValue = interfaceTag.getAttribute("hide").toBool();
		if(hideValue)
			gui.hideInterface(interfaceName);
	}
}

void XmlGuiParser::parseWidgetAttributes(const Xml& widgetTag, Widget& widget)
{
	if (widgetTag.hasAttribute("contentPath"))
	{
		auto path = widgetTag.getAttribute("contentPath").toString();
		mGameData->getTextures().load(path);
		widget.setContentPath(path);
	}
	if (widgetTag.hasAttribute("origin"))
	{
		
		widget.setOrigin(getVector(widgetTag, "origin"));
	}
	if (widgetTag.hasAttribute("position"))
	{
		PH_LOG_INFO("POSI_START");
		widget.setPosition(getVector(widgetTag, "position"));
		PH_LOG_INFO("POSI_END");
	}
	if (widgetTag.hasAttribute("scale"))
		widget.scale(getVector(widgetTag, "scale"));
	if (widgetTag.hasAttribute("alpha"))
		widget.setAlpha(widgetTag.getAttribute("alpha").toUnsigned());
		
	if (mActionsParser)
	{
		if (widgetTag.hasAttribute("onButtonPressed"))
		{
			auto action = widgetTag.getAttribute("onButtonPressed").toString();
			widget.addBehavior(BehaviorType::onPressed, mActionsParser->getGuiAction(*mGameData, action));
		}
		if (widgetTag.hasAttribute("onButtonReleased"))
		{
			auto action = widgetTag.getAttribute("onButtonReleased").toString();
			widget.addBehavior(BehaviorType::onReleased, mActionsParser->getGuiAction(*mGameData, action));
		}
		if (widgetTag.hasAttribute("onButtonUpdate"))
		{
			auto action = widgetTag.getAttribute("onButtonUpdate").toString();
			widget.addBehavior(BehaviorType::onUpdate, mActionsParser->getGuiAction(*mGameData, action));
		}
	}
}

void XmlGuiParser::parseTextWidgetAttributes(const Xml& textWidgetTag, TextWidget& widget)
{
	if (textWidgetTag.hasAttribute("fontPath"))
	{
		auto path = textWidgetTag.getAttribute("fontPath").toString();
		mGameData->getFonts().load(path);
		widget.setFontPath(path);
	}
	if (textWidgetTag.hasAttribute("textPosition"))
		widget.setTextPosition(getVector(textWidgetTag, "textPosition"));
	if (textWidgetTag.hasAttribute("color"))
		widget.setColor(getColor(textWidgetTag));
	if (textWidgetTag.hasAttribute("text"))
		widget.setString(textWidgetTag.getAttribute("text").toString());
	if (textWidgetTag.hasAttribute("characterSize"))
		widget.setCharacterSize(textWidgetTag.getAttribute("characterSize").toUnsigned());
	if (textWidgetTag.hasAttribute("textOrigin"))
		widget.setTextOrigin(getVector(textWidgetTag, "textOrigin"));
	if (textWidgetTag.hasAttribute("textAlpha"))
		widget.setTextAlpha(textWidgetTag.getAttribute("textAlpha").toUnsigned());
	if (textWidgetTag.hasAttribute("scaleText"))
		widget.scaleText(getVector(textWidgetTag, "scaleText"));
	if (textWidgetTag.hasAttribute("scrollingEffect"))
		widget.setScrollingEffect(textWidgetTag.getAttribute("scrollingEffect").toBool());
}

void XmlGuiParser::parseSliderWidgetAttributes(const Xml& widgetTag, SliderWidget& widget)
{
	if (widgetTag.hasAttribute("contentPathSlider"))
	{
		auto path = widgetTag.getAttribute("contentPathSlider").toString();
		mGameData->getTextures().load(path);
		widget.createSlider(path);
	}
	else throwMissingAttributeException("contentPathSlider");
}

void XmlGuiParser::parseWidgetChildren(const Xml& widgetTag, Widget& widget)
{
	auto widgets = widgetTag.getChildren("widget");
	for (auto const& childTag : widgets)
	{
		auto name = childTag.getAttribute("name").toString();
		auto childWidget = new Widget;
		widget.addWidget(name, childWidget);
		parseWidgetAttributes(childTag, *childWidget);
		parseWidgetChildren(childTag, *childWidget);
	}

	auto sliderWidget = widgetTag.getChildren("sliderWidget");
	for (auto const& childTag : sliderWidget)
	{
		auto name = childTag.getAttribute("name").toString();
		auto childWidget = new SliderWidget;
		widget.addWidget(name, childWidget);
		parseWidgetAttributes(childTag, *childWidget);
		parseSliderWidgetAttributes(childTag, *childWidget);
	}

	auto textWidgets = widgetTag.getChildren("textWidget");
	for (auto const& childTag : textWidgets)
	{
		auto name = childTag.getAttribute("name").toString();
		auto childWidget = new TextWidget;
		widget.addWidget(name, childWidget);
		parseWidgetAttributes(childTag, *childWidget);
		parseTextWidgetAttributes(childTag, *childWidget);
		parseWidgetChildren(childTag, *childWidget);
	}
}

sf::Vector2f XmlGuiParser::getVector(const Xml& widgetTag, const std::string& attributeName)
{
	auto pair = splitString(widgetTag.getAttribute(attributeName).toString());
		
	if (pair.second.empty())
	{
		auto value = std::stof(pair.first);
		return { value, value };
	}
	else
	{
		return { std::stof(pair.first),
					std::stof(pair.second) };
	}
}

std::pair<std::string, std::string> XmlGuiParser::splitString(const std::string& attributeValue)
{
	auto commaPos = attributeValue.find(',');
	std::pair<std::string, std::string> splittedAction;
	splittedAction.first = attributeValue.substr(0, commaPos);
	splittedAction.second = attributeValue.substr(commaPos + 1);
	return splittedAction;
}
	
sf::Color XmlGuiParser::getColor(const Xml& widgetTag)
{
	auto colorStr = widgetTag.getAttribute("color").toString();

	if (colorStr.substr(0, 4) == "rgba")
		return parseRGBA(colorStr);
	else if (colorStr.substr(0, 3) == "rgb")
		return parseRGB(colorStr);
	else if (colorStr == "black")
		return sf::Color::Black;
	else if (colorStr == "white")
		return sf::Color::White;
	else if (colorStr == "red")
		return sf::Color::Red;
	else if (colorStr == "green")
		return sf::Color::Green;
	else if (colorStr == "blue")
		return sf::Color::Blue;
	else if (colorStr == "yellow")
		return sf::Color::Yellow;
	else if (colorStr == "magenta")
		return sf::Color::Magenta;
	else if (colorStr == "cyan")
		return sf::Color::Cyan;
	else if (colorStr == "transparent")
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
