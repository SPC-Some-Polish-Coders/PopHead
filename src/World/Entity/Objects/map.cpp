#include "map.hpp"
#include "Base/gameData.hpp"
#include "Utilities/xml.hpp"
#include "Utilities/csv.hpp"

using PopHead::World::Entity::Map;
using PopHead::Utilities::Xml;

Map::Map(PopHead::Base::GameData* gameData, std::string name, const std::string& xmlFilename, float scale)
	: Object(gameData, name, Renderer::LayerID::floorEntities)
{
	Xml document;
	document.loadFromFile(xmlFilename);

	const Xml mapNode = document.getChild("map");

	const Xml tilesetNode = mapNode.getChild("tileset");
	const int tileWidth = tilesetNode.getAttribute("tilewidth").toInt();
	const int tileHeight = tilesetNode.getAttribute("tileheight").toInt();

	const Xml imageNode = tilesetNode.getChild("image");
	std::string source = imageNode.getAttribute("source").toString();
	source = Utilities::Parser::toFilename(source, '/');

	const Xml layerNode = mapNode.getChild("layer");
	const unsigned layerWidth = layerNode.getAttribute("width").toUnsigned();
	const unsigned layerHeight = layerNode.getAttribute("height").toUnsigned();
	mSprites.reserve(layerWidth * layerHeight);

	const Xml dataNode = layerNode.getChild("data");
	const std::string encoding = dataNode.getAttribute("encoding").toString();
	if (encoding != "csv")
		PH_EXCEPTION("Used unsupported data encoding: " + encoding);
	const std::vector<unsigned> values = Utilities::Csv::toUnsigneds(dataNode.toString());

	// TODO: Convert values from 1D to 2D (left, top)
	for (unsigned value : values) {
		if (value) {
			// TODO: Move map resources path to some better place or make it a static const for example?
			sf::Sprite sprite(
				mGameData->getTextures().get("resources/textures/map/" + source),
				sf::IntRect(0, 0, tileWidth, tileHeight)
			);
			sprite.setScale(scale, scale);
			mSprites.push_back(sprite);
		}
	}
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const sf::Sprite& sprite : mSprites)
		target.draw(sprite, states);
}
