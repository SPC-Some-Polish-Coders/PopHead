#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_MAP_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_MAP_H_

#include "World/Entity/object.hpp"
#include "Utilities/xml.hpp"
#include <string>
#include <vector>

namespace ph{

class Map : public Object
{
public:
    Map(GameData* gameData, std::string name, float scale = 1);

	void loadFromFile(const std::string& filename);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	struct TilesetsData {
		std::vector<std::string> sources;
		std::vector<unsigned> columnsCounts;
		std::vector<unsigned> globalIds;
		std::vector<unsigned> tileCounts;
	};

	TilesetsData getTilesetsData(const std::vector<Xml>& tilesetNodes) const;

	void loadTiles(const std::vector<unsigned>& values,
				   const TilesetsData& tilesets,
				   sf::Vector2u mapSize,
				   sf::Vector2u tileSize);

	bool hasTile(unsigned value) const { return value != 0; }

	std::size_t findTilesetIndex(unsigned value, const TilesetsData& tilesets) const;

	const std::string pathToMapTextures = "textures/map/";
	std::vector<sf::Sprite> mTiles;
};

}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_MAP_H_
