#include "xmlMapParser.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"
#include "Utilities/xml.hpp"

namespace ph {

void XmlMapParser::parseFile(GameData* const gameData, const std::string& fileName)
{
	PH_LOG_INFO("Map file (" + fileName + ") is being parsed.");
		
	Xml mapFile;
	mapFile.loadFromFile(fileName);
	const Xml mapNode = mapFile.getChild("map");
	checkMapSupport(mapNode);
	const sf::Vector2u mapSize = getMapSize(mapNode);
	gameData->getAIManager().registerMapSize(mapSize);
	const sf::Vector2u tileSize = getTileSize(mapNode);

	auto& map = gameData->getMap();
	map.load(fileName, {mapSize, tileSize});
}

void XmlMapParser::checkMapSupport(const Xml& mapNode) const
{
	const std::string orientation = mapNode.getAttribute("orientation").toString();
	if(orientation != "orthogonal")
		PH_EXCEPTION("Used unsupported map orientation: " + orientation);
	const std::string infinite = mapNode.getAttribute("infinite").toString();
	if(infinite != "0")
		PH_EXCEPTION("Infinite maps are not supported");
}

sf::Vector2u XmlMapParser::getMapSize(const Xml& mapNode) const
{
	return sf::Vector2u(
		mapNode.getAttribute("width").toUnsigned(),
		mapNode.getAttribute("height").toUnsigned()
	);
}

sf::Vector2u XmlMapParser::getTileSize(const Xml& mapNode) const
{
	return sf::Vector2u(
		mapNode.getAttribute("tilewidth").toUnsigned(),
		mapNode.getAttribute("tileheight").toUnsigned()
	);
}


}
