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

	const Xml tilesetNode = mapNode.getChild("tileset");
	const sf::Vector2u tileSize(
		tilesetNode.getAttribute("tilewidth").toUnsigned(),
		tilesetNode.getAttribute("tileheight").toUnsigned()
	);
	const unsigned tilesetColumns = tilesetNode.getAttribute("columns").toUnsigned();

	const Xml imageNode = tilesetNode.getChild("image");
	std::string source = imageNode.getAttribute("source").toString();
	source = Utilities::Path::toFilename(source, '/');

	const Xml layerNode = mapNode.getChild("layer");
	const unsigned layerColumns = layerNode.getAttribute("width").toUnsigned();
	const unsigned layerRows = layerNode.getAttribute("height").toUnsigned();
	mSprites.reserve(layerColumns * layerRows);

	const Xml dataNode = layerNode.getChild("data");
	const std::string encoding = dataNode.getAttribute("encoding").toString();
	if (encoding != "csv")
		PH_EXCEPTION("Used unsupported data encoding: " + encoding);
	const std::vector<unsigned> values = Utilities::Csv::toUnsigneds(dataNode.toString());

	unsigned i = 0;
	for (unsigned value : values) {
		if (value--) {		
			// TODO: Fix bug with tileset loading. Use tilesetColumns instead of layerColumns or something like that?
			const sf::Vector2u rect = Utilities::Math::toTwoDimensional(value, tilesetColumns);
			sf::Sprite sprite(
				mGameData->getTextures().get("resources/textures/map/" + source), // TODO: Move map resources path to some better place or make it a static const for example?
				sf::IntRect(sf::Vector2i(rect), static_cast<sf::Vector2i>(tileSize))
			);
			//sprite.setScale(scale, scale);
			//const sf::Vector2f position((i % layerColumns) * tileWidth, (i / layerColumns) * tileHeight);
			//sprite.setPosition(position);
			mSprites.push_back(sprite);
		}
		++i;
	}
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const sf::Sprite& sprite : mSprites)
		target.draw(sprite, states);
}
