#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace ph {

class GameData;
class Xml;

class XmlMapParser
{
public:
	void parseFile(GameData* const gameData, const std::string& fileName);

private:
	void checkMapSupport(const Xml& mapNode) const;
	sf::Vector2u getMapSize(const Xml& mapNode) const;
	sf::Vector2u getTileSize(const Xml& mapNode) const;
};

}
