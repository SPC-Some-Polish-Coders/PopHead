#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_MAP_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_MAP_H_

#include "World/Entity/object.hpp"
#include <string>
#include <vector>

namespace PopHead{
namespace World{
namespace Entity{


class Map : public Object
{
public:
    Map(Base::GameData* gameData, std::string name, const std::string& xmlFilename, float scale = 1);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	struct TilesetsData {
		std::vector<std::string> sources;
		std::vector<unsigned> columnsAmounts;
	};

	std::vector<sf::Sprite> mSprites;
};


}}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_MAP_H_
