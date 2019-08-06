#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace ph {

class TilesetsData;
class TilesData;
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
	std::vector<Xml> getTilesetNodes(const Xml& mapNode) const;
	auto getTilesetsData(const std::vector<Xml>& tilesetNodes) const -> const TilesetsData;
	auto getTilesData(const std::vector<Xml>& tileNodes) const -> TilesData;
	std::vector<Xml> getLayerNodes(const Xml& mapNode) const;
};

}
