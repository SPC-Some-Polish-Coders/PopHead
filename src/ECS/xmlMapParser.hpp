#pragma once

#include "entitiesTemplateStorage.hpp"
#include "Resources/textureHolder.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include <entt/entity/registry.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string>

namespace ph {

class AIManager;
class Xml;

struct GeneralMapInfo
{
	Vec2 mapSize;
	Vec2 tileSize;
	float nrOfChunks;
	float nrOfChunksInOneRow;
	float nrOfChunksInOneColumn;
};

struct Pit
{
	FloatRect bounds;
	bool exists = false;
};

struct TilesData
{
	u32 firstGlobalTileId;
	std::vector<u32> ids;
	std::vector<std::vector<FloatRect>> lightWalls;
	std::vector<std::vector<FloatRect>> rectCollisions;
	std::vector<std::vector<component::BodyCircle>> circleCollisions;
	std::vector<Pit> pits;
	std::vector<bool> puzzleGridRoads;
};

struct TilesetsData
{
	std::vector<u32> firstGlobalTileIds;
	std::vector<u32> tileCounts;
	std::vector<u32> columnsCounts;
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
	void createChunk(Vec2 chunkPos, const std::vector<u32>& globalTileIds, const TilesetsData&, const GeneralMapInfo&,
	                 u8 z, AIManager&, bool outdoor);
	size_t findTilesetIndex(u32 globalTileId, const TilesetsData& tilesets) const;
	size_t findTilesIndex(u32 firstGlobalTileId, const std::vector<TilesData>& tilesData) const;
	void createDebugName(entt::entity entity, const char* name) const;

private:
	std::vector<component::RenderChunk> mRenderChunks;
	std::vector<component::MultiStaticCollisionBody> mChunkCollisions;
	std::vector<Vec2i> mAlreadyCreatedPuzzleGridRoadChunks;
	DenialAreas mDenialAreas;
	entt::registry* mGameRegistry;
	EntitiesTemplateStorage* mTemplates;
	inline static float sChunkSize = 12.f;
	inline static u8 sLowestLayerZ = 200; 
};

}
