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
	sf::Vector2f mapSize;
	sf::Vector2f tileSize;
	float nrOfChunks;
	float nrOfChunksInOneRow;
	float nrOfChunksInOneColumn;
	bool isMapInfinite;
};

struct TilesData
{
	unsigned firstGlobalTileId;
	std::vector<unsigned> ids;
	std::vector<std::vector<FloatRect>> bounds;
	std::vector<std::vector<FloatRect>> lightWalls;
};

struct TilesetsData
{
	std::vector<unsigned> firstGlobalTileIds;
	std::vector<unsigned> tileCounts;
	std::vector<unsigned> columnsCounts;
	std::vector<TilesData> tilesData;
	std::string tilesetFileName;
};

struct DenialAreas
{
	std::vector<FloatRect> collisions;
	std::vector<FloatRect> lightWalls;
	std::vector<FloatRect> collisionsAndLightWalls;
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
	void createInfiniteMapChunk(sf::Vector2f chunkPos, const std::vector<unsigned>& globalTileIds, const TilesetsData&, const GeneralMapInfo&,
	                 unsigned char z, AIManager&);
	void createFinitMapLayer(const std::vector<unsigned>& globalTileIds, const TilesetsData&, const GeneralMapInfo&,
	                 unsigned char z, AIManager&);
	std::size_t findTilesetIndex(const unsigned globalTileId, const TilesetsData& tilesets) const;
	std::size_t findTilesIndex(const unsigned firstGlobalTileId, const std::vector<TilesData>& tilesData) const;

private:
	std::vector<component::RenderChunk> mRenderChunks;
	std::vector<component::MultiStaticCollisionBody> mChunkCollisions;
	DenialAreas mDenialAreas;
	entt::registry* mGameRegistry;
	EntitiesTemplateStorage* mTemplates;
	inline static float sChunkSize = 12.f;
	inline static unsigned char sLowestLayerZ = 200; 
};

}
