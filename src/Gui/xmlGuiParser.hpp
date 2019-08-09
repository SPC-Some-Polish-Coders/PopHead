#pragma once

#include "guiActionsParser.hpp"

#include <memory>
#include <string>

#include <SFML/System/Vector2.hpp>
#include "SFML/Graphics/Color.hpp"

namespace ph {

	class GameData;
	class Xml;
	class GUI;
	class Widget;
	class TextWidget;

	class XmlGuiParser
	{
	public:
		static void setActionsParser(std::unique_ptr<GuiActionsParser> actionsParser);

		void parseFile(GameData* const gameData, const std::string& fileName);
		void parseInterface(const Xml& interfaceTag, GUI& gui);
		void parseWidgetAttributes(const Xml& widgetTag, Widget& widget);
		void parseTextWidgetAttributes(const Xml& textWidgetTag, TextWidget& widget);
		void parseWidgetChildren(const Xml& widgetTag, Widget& widget);
		
		sf::Vector2f getVector(const Xml& widgetTag, const std::string& attributeName);
		std::pair<std::string, std::string> splitString(const std::string& attributeValue);
		sf::Color getColor(const Xml& widgetTag);

	private:
		GameData* mGameData;
		static std::unique_ptr<GuiActionsParser> mActionsParser;
	};
}
