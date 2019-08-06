#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace ph {

struct GeneralMapInfo;
struct TilesetsData;
struct TilesData;
class GameData;
class Xml;

using GlobalTileIds = std::vector<unsigned>;
using AllLayersGlobalTileIds = std::vector<GlobalTileIds>;

class XmlMapParser
{
public:
	void parseFile(GameData* const gameData, const std::string& fileName);

private:
	void checkMapSupport(const Xml& mapNode) const;
	auto getGeneralMapInfo(const Xml& mapNode) const -> GeneralMapInfo;
	sf::Vector2u getMapSize(const Xml& mapNode) const;
	sf::Vector2u getTileSize(const Xml& mapNode) const;
	std::vector<Xml> getTilesetNodes(const Xml& mapNode) const;
	auto getTilesetsData(const std::vector<Xml>& tilesetNodes) const -> const TilesetsData;
	auto getTilesData(const std::vector<Xml>& tileNodes) const -> TilesData;
	std::vector<Xml> getLayerNodes(const Xml& mapNode) const;
	auto getAllLayersGlobalTileIds(const std::vector<Xml>& layerNodes) const -> AllLayersGlobalTileIds;
	std::vector<unsigned> toGlobalTileIds(const Xml& dataNode) const;
};

}
