#pragma once

#include "entitiesTemplateStorage.hpp"
#include "Resources/textureHolder.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph {

class AIManager;
class Texture;
class Xml;

struct GeneralMapInfo
{
	Vec2 mapSize;
	Vec2 tileSize;
	float nrOfChunks;
	float nrOfChunksInOneRow;
	float nrOfChunksInOneColumn;
};

enum TileFlags
{
	TileFlag_SlicedEntity = (1 << 0),
	TileFlag_DummySliceOfSlicedEntity = (1 << 1),
	TileFlag_PuzzleGridRoad = (1 << 2),
	TileFlag_Pit = (1 << 3),
	TileFlag_Cactus = (1 << 4),
	TileFlag_Rock = (1 << 5),
};

struct TilesData
{
	u32 firstGlobalTileId;
	std::vector<u32> ids;
	std::vector<std::vector<FloatRect>> lightWalls;
	std::vector<std::vector<FloatRect>> rectCollisions;
	std::vector<std::vector<component::BodyCircle>> circleCollisions;
	std::vector<FloatRect> pitBounds;
	std::vector<u32> flags;
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
	void parseFile(const Xml& mapNode, AIManager& aiManager, entt::registry& gameRegistry,
	               EntitiesTemplateStorage& templates, Texture* tilesetTexture);
private:
	auto getGeneralMapInfo(const Xml& mapNode) const -> GeneralMapInfo;
	
	auto getTilesetsData(const std::vector<Xml>& tilesetNodes) const -> const TilesetsData;
	auto getTilesData(const std::vector<Xml>& tileNodes) const -> TilesData;
	void createChunk(Vec2 chunkPos, const std::vector<u32>& globalTileIds, const TilesetsData&, const GeneralMapInfo&,
	                 u8 z, AIManager&, bool outdoor, const std::string& layerName, Texture* tilesetTexture);
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
};

}
