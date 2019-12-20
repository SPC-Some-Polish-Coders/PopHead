#pragma once

#include "entitiesTemplateStorage.hpp"
#include "Resources/resourceHolder.hpp"

#include <entt/entity/registry.hpp>
#include <SFML/Graphics.hpp>
#include <string>

namespace ph {

class AIManager;
class Xml;

struct GeneralMapInfo
{
	const sf::Vector2u mapSize;
	const sf::Vector2u tileSize;
};

struct TilesData
{
	unsigned firstGlobalTileId;
	std::vector<unsigned> ids;
	std::vector<sf::FloatRect> bounds;
};

struct TilesetsData
{
	std::vector<unsigned> firstGlobalTileIds;
	std::vector<unsigned> tileCounts;
	std::vector<unsigned> columnsCounts;
	std::vector<TilesData> tilesData;
	std::string tilesetFileName;
};

class XmlMapParser
{
public:
	void parseFile(const std::string& fileName, AIManager& aiManager, entt::registry& gameRegistry,
	               EntitiesTemplateStorage& templates, TextureHolder& textures);

private:
	void checkMapSupport(const Xml& mapNode) const;
	auto getGeneralMapInfo(const Xml& mapNode) const -> GeneralMapInfo;
	sf::Vector2u getMapSize(const Xml& mapNode) const;
	sf::Vector2u getTileSize(const Xml& mapNode) const;
	
	std::vector<Xml> getTilesetNodes(const Xml& mapNode) const;
	auto getTilesetsData(const std::vector<Xml>& tilesetNodes) const -> const TilesetsData;
	auto getTilesData(const std::vector<Xml>& tileNodes) const -> TilesData;
	std::vector<Xml> getLayerNodes(const Xml& mapNode) const;
	void parserMapLayers(const std::vector<Xml>& layerNodes, const TilesetsData& tilesets, const GeneralMapInfo& info);
	std::vector<unsigned> toGlobalTileIds(const Xml& dataNode) const;
	
	void createLayer(const std::vector<unsigned>& globalTileIds, const TilesetsData&, const GeneralMapInfo&, unsigned char z);
	bool hasTile(unsigned globalTileId) const;
	std::size_t findTilesetIndex(const unsigned globalTileId, const TilesetsData& tilesets) const;
	std::size_t findTilesIndex(const unsigned firstGlobalTileId, const std::vector<TilesData>& tilesData) const;
	void loadCollisionBodies(const unsigned tileId, const TilesData& tilesData, sf::Vector2f position);
	void createMapBorders(const GeneralMapInfo& mapInfo);

private:
	entt::registry* mGameRegistry;
	EntitiesTemplateStorage* mTemplates;
	TextureHolder* mTextures;
};

}
