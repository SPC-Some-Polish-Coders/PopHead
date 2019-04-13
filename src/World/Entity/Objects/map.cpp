#include "map.hpp"

using PopHead::World::Entity::Map;


Map::Map(Base::GameData* gameData, std::string name, const sf::Texture& texture)
:Object(gameData, name, Renderer::LayerID::staticEntities)
{
    mSprite.setTexture(texture);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
