#include "map.hpp"
#include "Base/gameData.hpp"
#include "Utilities/xml.hpp"
#include "Utilities/csv.hpp"
#include "Utilities/math.hpp"

using PopHead::World::Entity::Map;
using PopHead::Utilities::Xml;

Map::Map(PopHead::Base::GameData* gameData, std::string name, const std::string& xmlFilename, float scale)
	: Object(gameData, name, Renderer::LayerID::floorEntities)
{
	Xml document;
	document.loadFromFile(xmlFilename);

	const Xml mapNode = document.getChild("map");
	const sf::Vector2u mapSize(
		mapNode.getAttribute("width").toUnsigned(),
		mapNode.getAttribute("height").toUnsigned()
	);
	const sf::Vector2u tileSize(
		mapNode.getAttribute("tilewidth").toUnsigned(),
		mapNode.getAttribute("tileheight").toUnsigned()
	);

	const Xml tilesetNode = mapNode.getChild("tileset");
	const unsigned tilesetColumns = tilesetNode.getAttribute("columns").toUnsigned();

	const Xml imageNode = tilesetNode.getChild("image");
	std::string source = imageNode.getAttribute("source").toString();
	source = Utilities::Path::toFilename(source, '/');

	const Xml layerNode = mapNode.getChild("layer");
	const sf::Vector2u layerSize(
		layerNode.getAttribute("width").toUnsigned(),
		layerNode.getAttribute("height").toUnsigned()
	);
	mSprites.reserve(layerSize.x * layerSize.y);

	const Xml dataNode = layerNode.getChild("data");
	const std::string encoding = dataNode.getAttribute("encoding").toString();
	if (encoding != "csv")
		PH_EXCEPTION("Used unsupported data encoding: " + encoding);
	const std::vector<unsigned> values = Utilities::Csv::toUnsigneds(dataNode.toString());

	/*
		TODO:
		Move map resources path to some better place or make it a static const for example?
	*/
	for (unsigned value : values) {
		if (value--) {
			sf::Vector2u position = Utilities::Math::toTwoDimensional(value, tilesetColumns);
			position.x *= tileSize.x;
			position.y *= tileSize.y;
			const sf::IntRect rect(
				static_cast<sf::Vector2i>(position),
				static_cast<sf::Vector2i>(tileSize)
			);
			const sf::Sprite sprite(mGameData->getTextures().get("resources/textures/map/" + source), rect);
			//sprite.setScale(scale, scale);
			//const sf::Vector2f position((i % layerColumns) * tileWidth, (i / layerColumns) * tileHeight);
			//sprite.setPosition(position);
			mSprites.push_back(sprite);
		}
	}
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const sf::Sprite& sprite : mSprites)
		target.draw(sprite, states);
}
