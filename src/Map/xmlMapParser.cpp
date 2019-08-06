#include "xmlMapParser.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"
#include "Utilities/xml.hpp"
#include "Utilities/filePath.hpp"

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
	const std::vector<Xml> tilesetNodes = getTilesetNodes(mapNode);
	const TilesetsData tilesetsData = getTilesetsData(tilesetNodes);
	const std::vector<Xml> layerNodes = getLayerNodes(mapNode);

	auto& map = gameData->getMap();
	map.load({mapSize, tileSize}, tilesetsData, layerNodes);
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

std::vector<Xml> XmlMapParser::getTilesetNodes(const Xml& mapNode) const
{
	const std::vector<Xml> tilesetNodes = mapNode.getChildren("tileset");
	if(tilesetNodes.size() == 0)
		PH_LOG_WARNING("Map doesn't have any tilesets");
	return tilesetNodes;
}

auto XmlMapParser::getTilesetsData(const std::vector<Xml>& tilesetNodes) const -> const TilesetsData
{
	TilesetsData tilesets;
	tilesets.firstGlobalTileIds.reserve(tilesetNodes.size());
	tilesets.tileCounts.reserve(tilesetNodes.size());
	tilesets.columnsCounts.reserve(tilesetNodes.size());
	for(Xml tilesetNode : tilesetNodes) {
		const unsigned firstGlobalTileId = tilesetNode.getAttribute("firstgid").toUnsigned();
		tilesets.firstGlobalTileIds.push_back(firstGlobalTileId);
		if(tilesetNode.hasAttribute("source")) {
			std::string tilesetNodeSource = tilesetNode.getAttribute("source").toString();
			tilesetNodeSource = FilePath::toFilename(tilesetNodeSource, '/');
			PH_LOG_INFO("Detected not embedded tileset in Map: " + tilesetNodeSource);
			Xml tilesetDocument;
			tilesetDocument.loadFromFile(tilesetNodeSource);
			tilesetNode = tilesetDocument.getChild("tileset");
		}
		tilesets.tileCounts.push_back(tilesetNode.getAttribute("tilecount").toUnsigned());
		tilesets.columnsCounts.push_back(tilesetNode.getAttribute("columns").toUnsigned());
		const Xml imageNode = tilesetNode.getChild("image");
		tilesets.tilesetFileName = FilePath::toFilename(imageNode.getAttribute("source").toString(), '/');
		const std::vector<Xml> tileNodes = tilesetNode.getChildren("tile");
		TilesData tilesData = getTilesData(tileNodes);
		tilesData.firstGlobalTileId = firstGlobalTileId;
		tilesets.tilesData.push_back(tilesData);
	}
	return tilesets;
}

auto XmlMapParser::getTilesData(const std::vector<Xml>& tileNodes) const -> TilesData
{
	TilesData tilesData{};
	tilesData.ids.reserve(tileNodes.size());
	tilesData.bounds.reserve(tileNodes.size()); // WARNING: Change it when there would be possibility to have more than one CollisionBody per tile?
	for(const Xml& tileNode : tileNodes) {
		tilesData.ids.push_back(tileNode.getAttribute("id").toUnsigned());
		const Xml objectGroupNode = tileNode.getChild("objectgroup");
		const Xml objectNode = objectGroupNode.getChild("object");
		const sf::FloatRect bounds(
			objectNode.getAttribute("x").toFloat(),
			objectNode.getAttribute("y").toFloat(),
			objectNode.hasAttribute("width") ? objectNode.getAttribute("width").toFloat() : 0.f,
			objectNode.hasAttribute("height") ? objectNode.getAttribute("height").toFloat() : 0.f
		);
		if(!(bounds.width == 0.f && bounds.height == 0.f))
			tilesData.bounds.push_back(bounds);
	}
	return tilesData;
}

std::vector<Xml> XmlMapParser::getLayerNodes(const Xml& mapNode) const
{
	const std::vector<Xml> layerNodes = mapNode.getChildren("layer");
	if(layerNodes.size() == 0)
		PH_LOG_WARNING("Map doesn't have any layers");
	return layerNodes;
}

}
