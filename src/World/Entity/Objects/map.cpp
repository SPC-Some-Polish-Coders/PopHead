#include "map.hpp"

using PopHead::World::Entity::Map;


Map::Map(PopHead::Base::GameData* gameData, std::string name, const sf::Texture& texture, float scale)
	:Object(gameData, name, Renderer::LayerID::floorEntities)
{
	mSprite.setTexture(texture);
	mSprite.setScale(scale, scale);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}
