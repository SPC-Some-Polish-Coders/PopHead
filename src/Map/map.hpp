#pragma once

#include "chunkMap.hpp"
#include "Utilities/xml.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

namespace ph{

class GameData;

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

class Map
{
public:
	using GlobalTileIds = std::vector<unsigned>;
	using AllLayersGlobalTileIds = std::vector<GlobalTileIds>;

public:
    Map();

	void load(const GeneralMapInfo&, const TilesetsData&, const AllLayersGlobalTileIds&);

    void draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::FloatRect cameraBounds) const;

	void setGameData(GameData* const gameData) { mGameData = gameData; };

private:
	void createChunkMap(const TilesetsData& tilesets, const GeneralMapInfo&);

	void createAllLayers(const AllLayersGlobalTileIds&, const TilesetsData&, const GeneralMapInfo&);

	std::vector<unsigned> toGlobalTileIds(const Xml& dataNode) const;

	void createLayer(const std::vector<unsigned>& globalTileIds, const TilesetsData& tilesets, const GeneralMapInfo& info);

	bool hasTile(unsigned globalTileId) const { return globalTileId != 0; }

	std::size_t findTilesetIndex(const unsigned globalTileId, const TilesetsData& tilesets) const;

	std::size_t findTilesIndex(const unsigned firstGlobalTileId, const std::vector<TilesData>& tilesData) const;

	void loadCollisionBodies(const unsigned tileId, const TilesData& tilesData, sf::Vector2f position);

	void createMapBorders(const GeneralMapInfo& mapInfo);

private:
	inline static const std::string pathToTilesetsDirectory = "textures/map/";
	std::unique_ptr<ChunkMap> mChunkMap;
	GameData* mGameData;
};

}
