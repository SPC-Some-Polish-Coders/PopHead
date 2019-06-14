#include "map.hpp"
#include "Base/gameData.hpp"
#include "Utilities/debug.hpp"
#include "Utilities/csv.hpp"
#include "Utilities/math.hpp"

ph::Map::Map(GameData* gameData, std::string name)
	:Object(gameData, name, LayerID::floorEntities)
{
}

void ph::Map::loadFromFile(const std::string& filename)
{
	Xml document;
	document.loadFromFile(filename);
	const Xml mapNode = document.getChild("map");
	checkMapSupport(mapNode);
	const sf::Vector2u mapSize = getMapSize(mapNode);
	const sf::Vector2u tileSize = getTileSize(mapNode);
	const std::vector<Xml> tilesetNodes = getTilesetNodes(mapNode);
	const TilesetsData tilesets = getTilesetsData(tilesetNodes);
	const std::vector<Xml> layerNodes = getLayerNodes(mapNode);
	mTiles.reserve(mapSize.x * mapSize.y * layerNodes.size());
	for (const Xml& layerNode : layerNodes) {
		const Xml dataNode = layerNode.getChild("data");
		const std::vector<unsigned> globalTileIds = toGlobalTileIds(dataNode);
		loadTiles(globalTileIds, tilesets, mapSize, tileSize);
	}
}

void ph::Map::checkMapSupport(const Xml& mapNode) const
{
	const std::string orientation = mapNode.getAttribute("orientation").toString();
	if (orientation != "orthogonal")
		PH_EXCEPTION("Used unsupported map orientation: " + orientation);

	const std::string infinite = mapNode.getAttribute("infinite").toString();
	if (infinite != "0")
		PH_EXCEPTION("Infinite maps are not supported");
}

sf::Vector2u ph::Map::getMapSize(const Xml& mapNode) const
{
	return sf::Vector2u(
		mapNode.getAttribute("width").toUnsigned(),
		mapNode.getAttribute("height").toUnsigned()
	);
}

sf::Vector2u ph::Map::getTileSize(const Xml& mapNode) const
{
	return sf::Vector2u(
		mapNode.getAttribute("tilewidth").toUnsigned(),
		mapNode.getAttribute("tileheight").toUnsigned()
	);
}

std::vector<ph::Xml> ph::Map::getTilesetNodes(const Xml& mapNode) const
{
	const std::vector<Xml> tilesetNodes = mapNode.getChildren("tileset");
	if (tilesetNodes.size() == 0)
		PH_LOG(LogType::Warning, "Map doesn't have any tilesets");
	return tilesetNodes;
}

ph::Map::TilesetsData ph::Map::getTilesetsData(const std::vector<Xml>& tilesetNodes) const
{
	/*
		TODO:
		What if tileset is self-closing tag (firstgid and source is defined, but he is in different file)?
		- (BEST) Do something with Xml impl to check if there is source attribute defined? ->
			* return iterator in getAttribute(name)?
			* return std::pair in getAttribute(name)?
			* return struct in getAttribute(name)?
			* make output argument?
			* make method hasAttribute(name)? -> bad performance (double find or hard impl based on temp buffer)
		- Assume that there is not such? -> Maybe it would be better to just allow them.
		- Try to find it and catch corresponding exception by checking error message
			(much better: define proper exception type in Xml impl)?
	*/
	TilesetsData tilesets;
	tilesets.sources.reserve(tilesetNodes.size());
	tilesets.columnsCounts.reserve(tilesetNodes.size());
	tilesets.firstGlobalTileIds.reserve(tilesetNodes.size());
	tilesets.tileCounts.reserve(tilesetNodes.size());
	for (const Xml& tilesetNode : tilesetNodes) {
		tilesets.columnsCounts.push_back(tilesetNode.getAttribute("columns").toUnsigned());
		tilesets.firstGlobalTileIds.push_back(tilesetNode.getAttribute("firstgid").toUnsigned());
		tilesets.tileCounts.push_back(tilesetNode.getAttribute("tilecount").toUnsigned());
		const Xml imageNode = tilesetNode.getChild("image");
		std::string source = imageNode.getAttribute("source").toString();
		source = Path::toFilename(source, '/');
		tilesets.sources.push_back(source);
	}
	return tilesets;
}

std::vector<ph::Xml> ph::Map::getLayerNodes(const Xml& mapNode) const
{
	const std::vector<Xml> layerNodes = mapNode.getChildren("layer");
	if (layerNodes.size() == 0)
		PH_LOG(LogType::Warning, "Map doesn't have any layers");
	return layerNodes;
}

std::vector<unsigned> ph::Map::toGlobalTileIds(const Xml& dataNode) const
{
	const std::string encoding = dataNode.getAttribute("encoding").toString();
	if (encoding == "csv")
		return Csv::toUnsigneds(dataNode.toString());
	PH_EXCEPTION("Used unsupported data encoding: " + encoding);
}

void ph::Map::loadTiles(
	const std::vector<unsigned>& globalTileIds,
	const TilesetsData& tilesets,
	sf::Vector2u mapSize,
	sf::Vector2u tileSize)
{
	for (std::size_t i = 0; i < globalTileIds.size(); ++i) {
		if (hasTile(globalTileIds[i])) {
			const std::size_t j = findTilesetIndex(globalTileIds[i], tilesets);
			if (j == std::string::npos) {
				PH_LOG(LogType::Warning, "It was not possible to find tileset for " + globalTileIds[i]);
				continue;
			}
			const unsigned tileId = globalTileIds[i] - tilesets.firstGlobalTileIds[j];
			sf::Vector2u tileRectPosition = Math::toTwoDimensional(tileId, tilesets.columnsCounts[j]);
			tileRectPosition.x *= tileSize.x;
			tileRectPosition.y *= tileSize.y;
			const sf::IntRect tileRect(
				static_cast<sf::Vector2i>(tileRectPosition),
				static_cast<sf::Vector2i>(tileSize)
			);
			sf::Sprite tile(mGameData->getTextures().get(pathToMapTextures + tilesets.sources[j]), tileRect);
			sf::Vector2f position(Math::toTwoDimensional(i, mapSize.x));
			position.x *= tileSize.x;
			position.y *= tileSize.y;
			tile.setPosition(position);
			mTiles.push_back(tile);
		}
	}
}

std::size_t ph::Map::findTilesetIndex(unsigned globalTileId, const TilesetsData& tilesets) const
{
	for (std::size_t i = 0; i < tilesets.firstGlobalTileIds.size(); ++i) {
		const unsigned firstGlobalTileId = tilesets.firstGlobalTileIds[i];
		const unsigned lastGlobalTileId = tilesets.firstGlobalTileIds[i] + tilesets.tileCounts[i] - 1;
		if (globalTileId >= firstGlobalTileId && globalTileId <= lastGlobalTileId)
			return i;
	}
	return std::string::npos;
}

void ph::Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const sf::Sprite& sprite : mTiles)
		target.draw(sprite, states);
}
