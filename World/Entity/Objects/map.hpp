#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_MAP_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_MAP_H_

#include "World/Entity/object.hpp"

namespace PopHead{
namespace World{
namespace Entity{


class Map : public Object
{
public:
    Map(Base::GameData* gameData, std::string name, const sf::Texture& texture, float scale = 1);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Sprite mSprite;
};


}}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_MAP_H_
