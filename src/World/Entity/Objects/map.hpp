#pragma once

#include "World/Entity/object.hpp"
#include "Utilities/xml.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include <string>
#include <vector>

namespace ph{

class Map : public Object
{
public:
    Map(GameData* gameData, std::string name);

	void loadFromFile(const std::string& filename);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	struct TilesetsData 
	{
		struct TilesData 
		{
			unsigned firstGlobalTileId;
			std::vector<unsigned> ids;
			std::vector<sf::FloatRect> bounds;
		};

		std::vector<unsigned> firstGlobalTileIds;
		std::vector<unsigned> tileCounts;
		std::vector<unsigned> columnsCounts;
		std::vector<std::string> sources;
		std::vector<TilesData> tiles;
	};

	void checkMapSupport(const Xml& mapNode) const;

	sf::Vector2u getMapSize(const Xml& mapNode) const;

	sf::Vector2u getTileSize(const Xml& mapNode) const;

	std::vector<Xml> getTilesetNodes(const Xml& mapNode) const;

	TilesetsData getTilesetsData(const std::vector<Xml>& tilesetNodes) const;

	std::vector<Xml> getLayerNodes(const Xml& mapNode) const;

	std::vector<unsigned> toGlobalTileIds(const Xml& dataNode) const;

	void loadTiles(
		const std::vector<unsigned>& globalTileIds,
		const TilesetsData& tilesets,
		sf::Vector2u mapSize,
		sf::Vector2u tileSize);

	bool hasTile(unsigned globalTileId) const { return globalTileId != 0; }

	std::size_t findTilesetIndex(unsigned globalTileId, const TilesetsData& tilesets) const;

	inline static const std::string pathToMapTextures = "textures/map/";
	std::vector<sf::Sprite> mTiles;
	std::vector<std::unique_ptr<CollisionBody>> mCollisionBodies;
};

}
