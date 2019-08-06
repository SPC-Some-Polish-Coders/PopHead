#pragma once

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
		void parseFile(GameData* const gameData, const std::string& fileName);
		void parseInterface(const Xml& interfaceTag, GUI& gui);
		void parseWidgetAttributes(const Xml& widgetTag, Widget& widget);
		void parseTextWidgetAttributes(const Xml& textWidgetTag, TextWidget& widget);
		void parseWidgetChildren(const Xml& widgetTag, Widget& widget);
		
		sf::Vector2f getWidgetOrigin(const Xml& widgetTag);
		sf::Vector2f getWidgetPosition(const Xml& widgetTag);
		sf::Vector2f getTextPosition(const Xml& widgetTag);
		sf::Color getColor(const Xml& widgetTag);

	private:
		GameData* mGameData;
	};
}
