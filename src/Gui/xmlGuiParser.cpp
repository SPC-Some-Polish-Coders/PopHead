#include "xmlGuiParser.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"
#include "gui.hpp"
#include "Utilities/xml.hpp"

namespace ph {

	void XmlGuiParser::parseFile(GameData* const gameData, const std::string& fileName)
	{
		PH_LOG_INFO("Gui file (" + fileName + ") is being parsed.");

		mGameData = gameData;

		auto& gui = gameData->getGui();
		gui.clearGUI();

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
		parseWidgetAttributes(interfaceTag, *interface);
		parseWidgetChildren(interfaceTag, *interface);
	}

	void XmlGuiParser::parseWidgetAttributes(const Xml& widgetTag, Widget& widget)
	{
		if (widgetTag.hasAttribute("contentPath"))
		{
			auto path = widgetTag.getAttribute("contentPath").toString();
			mGameData->getTextures().load(path);
			widget.setContentPath(path);
		}
		if (widgetTag.hasAttribute("originX") && widgetTag.hasAttribute("originY"))
			widget.setOrigin(getWidgetOrigin(widgetTag));
		if (widgetTag.hasAttribute("positionX") && widgetTag.hasAttribute("positionY"))
			widget.setPosition(getWidgetPosition(widgetTag));
	}

	void XmlGuiParser::parseTextWidgetAttributes(const Xml& textWidgetTag, TextWidget& widget)
	{
		if (textWidgetTag.hasAttribute("fontPath"))
		{
			auto path = textWidgetTag.getAttribute("fontPath").toString();
			mGameData->getFonts().load(path);
			widget.setFontPath(path);
		}
		if (textWidgetTag.hasAttribute("textPositionX") && textWidgetTag.hasAttribute("textPositionY"))
			widget.setTextPosition(getTextPosition(textWidgetTag));
		if (textWidgetTag.hasAttribute("color"))
			widget.setColor(getColor(textWidgetTag));
	}

	void XmlGuiParser::parseWidgetChildren(const Xml& widgetTag, Widget& widget)
	{
		auto widgets = widgetTag.getChildren("widget");
		for (auto& childTag : widgets)
		{
			auto name = childTag.getAttribute("name").toString();
			auto childWidget = new Widget;
			widget.addWidget(name, childWidget);
			parseWidgetAttributes(childTag, *childWidget);
			parseWidgetChildren(childTag, *childWidget);
		}

		auto textWidgets = widgetTag.getChildren("textWidget");
		for (auto& childTag : textWidgets)
		{
			auto name = childTag.getAttribute("name").toString();
			auto childWidget = new TextWidget;
			widget.addWidget(name, childWidget);
			parseWidgetAttributes(childTag, *childWidget);
			parseTextWidgetAttributes(childTag, *childWidget);
			auto str = childTag.toString();
			childWidget->setString(str);
		}
	}

	sf::Vector2f XmlGuiParser::getWidgetOrigin(const Xml& widgetTag)
	{
		return sf::Vector2f(widgetTag.getAttribute("originX").toFloat(),
							widgetTag.getAttribute("originY").toFloat());
	}

	sf::Vector2f XmlGuiParser::getWidgetPosition(const Xml& widgetTag)
	{
		return sf::Vector2f(widgetTag.getAttribute("positionX").toFloat(),
							widgetTag.getAttribute("positionY").toFloat());
	}

	sf::Vector2f XmlGuiParser::getTextPosition(const Xml& widgetTag)
	{
		return sf::Vector2f(widgetTag.getAttribute("textPositionX").toFloat(),
							widgetTag.getAttribute("textPositionY").toFloat());
	}
	
	sf::Color XmlGuiParser::getColor(const Xml& widgetTag)
	{
		auto colorStr = widgetTag.getAttribute("color").toString();

		if (colorStr == "black")
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
}
