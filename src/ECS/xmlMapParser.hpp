#pragma once

#include "entitiesTemplateStorage.hpp"
#include "Resources/resourceHolder.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include <entt/entity/registry.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace ph {

class AIManager;
class Xml;

struct GeneralMapInfo
{
	sf::Vector2u mapSize;
	sf::Vector2u tileSize;
	float nrOfChunks;
	float nrOfChunksInOneRow;
	float nrOfChunksInOneColumn;
	bool isInfinite;
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
	void parseFile(const Xml& mapNode, AIManager& aiManager, entt::registry& gameRegistry, EntitiesTemplateStorage& templates);
private:
	auto getGeneralMapInfo(const Xml& mapNode) const -> GeneralMapInfo;
	
	auto getTilesetsData(const std::vector<Xml>& tilesetNodes) const -> const TilesetsData;
	auto getTilesData(const std::vector<Xml>& tileNodes) const -> TilesData;
	std::vector<Xml> getLayerNodes(const Xml& mapNode) const;
	void parserMapLayers(const std::vector<Xml>& layerNodes, const TilesetsData&, const GeneralMapInfo&, AIManager&);
	
	void createLayer(const std::vector<unsigned>& globalTileIds, const TilesetsData&, const GeneralMapInfo&,
	                 unsigned char z, AIManager&);
	std::size_t findTilesetIndex(const unsigned globalTileId, const TilesetsData& tilesets) const;
	std::size_t findTilesIndex(const unsigned firstGlobalTileId, const std::vector<TilesData>& tilesData) const;
	void createMapBorders(const GeneralMapInfo& mapInfo);

private:
	std::vector<component::RenderChunk> mRenderChunks;
	std::vector<component::MultiStaticCollisionBody> mChunkCollisions;
	entt::registry* mGameRegistry;
	EntitiesTemplateStorage* mTemplates;
	inline static float sChunkSize = 12.f;
	inline static unsigned char sLowestLayerZ = 200; 
};

}
