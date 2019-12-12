#pragma once

#include <entt/entity/registry.hpp>
#include <SFML/Graphics.hpp>
#include <string>

namespace ph {

struct GeneralMapInfo;
struct TilesetsData;
struct TilesData;
class AIManager;
class Xml;

class XmlMapParser
{
public:
	void parseFile(const std::string& fileName, AIManager& aiManager, entt::registry& gameRegistry);

private:
	void checkMapSupport(const Xml& mapNode) const;
	auto getGeneralMapInfo(const Xml& mapNode) const -> GeneralMapInfo;
	sf::Vector2u getMapSize(const Xml& mapNode) const;
	sf::Vector2u getTileSize(const Xml& mapNode) const;
	std::vector<Xml> getTilesetNodes(const Xml& mapNode) const;
	auto getTilesetsData(const std::vector<Xml>& tilesetNodes) const -> const TilesetsData;
	auto getTilesData(const std::vector<Xml>& tileNodes) const -> TilesData;
	std::vector<Xml> getLayerNodes(const Xml& mapNode) const;
	//auto getAllLayersGlobalTileIds(const std::vector<Xml>& layerNodes) const -> AllLayersGlobalTileIds;
	void parserMapLayers(const std::vector<Xml>& layerNodes, entt::registry& gameRegistry);
	std::vector<unsigned> toGlobalTileIds(const Xml& dataNode) const;
};

}
