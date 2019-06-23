#include "map.hpp"
#include "gameData.hpp"
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
			* return std::optional?
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
	tilesets.firstGlobalTileIds.reserve(tilesetNodes.size());
	tilesets.tileCounts.reserve(tilesetNodes.size());
	tilesets.columnsCounts.reserve(tilesetNodes.size());
	tilesets.sources.reserve(tilesetNodes.size());
	for (const Xml& tilesetNode : tilesetNodes) {
		const unsigned firstGlobalTileId = tilesetNode.getAttribute("firstgid").toUnsigned();
		tilesets.firstGlobalTileIds.push_back(firstGlobalTileId);
		tilesets.tileCounts.push_back(tilesetNode.getAttribute("tilecount").toUnsigned());
		tilesets.columnsCounts.push_back(tilesetNode.getAttribute("columns").toUnsigned());
		const Xml imageNode = tilesetNode.getChild("image");
		std::string source = imageNode.getAttribute("source").toString();
		source = Path::toFilename(source, '/');
		tilesets.sources.push_back(source);

		const std::vector<Xml> tileNodes = tilesetNode.getChildren("tile");
		TilesetsData::TilesData tiles;
		tiles.ids.reserve(tileNodes.size());
		tiles.bounds.reserve(tileNodes.size()); // WARNING: Change it when there would be possibility to have more than one CollisionBody per tile?
		tiles.firstGlobalTileId = firstGlobalTileId;
		for (const Xml& tileNode : tileNodes) {
			tiles.ids.push_back(tileNode.getAttribute("id").toUnsigned());
			const Xml objectGroupNode = tileNode.getChild("objectgroup");
			/*
				TODO:
				* Could objectGroupNode contain more than one object?
				If that would be true there would be a possibility to have
				more than one CollisionBody per tile.
				(If it would be possible wrap some of them into one?).

				* Inform that other types are not allowed? Allow some of them?
				For example:
					- ellipse
					- point
					- polygon
					- polyline
					- text

				* Check whether objectNode has width and height attributes. 
				- If there is no width set it to 0
				- If there is no height set it to 0
				- If there is no width and height ignore this object
				(In Tiled when you create CollisionBody without width and height they are not created in .tmx file)
			*/
			const Xml objectNode = objectGroupNode.getChild("object");
			const sf::FloatRect bounds(
				objectNode.getAttribute("x").toFloat(),
				objectNode.getAttribute("y").toFloat(),
				objectNode.getAttribute("width").toFloat(),
				objectNode.getAttribute("height").toFloat()
			);
			tiles.bounds.push_back(bounds);
		}
		tilesets.tiles.push_back(tiles);
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
		const unsigned bitsInByte = 8;
		const unsigned flippedHorizontally = 0B1u << (sizeof(unsigned) * bitsInByte - 1);
		const unsigned flippedVertically = 0B1u << (sizeof(unsigned) * bitsInByte - 2);
		const unsigned flippedDiagonally = 0B1u << (sizeof(unsigned) * bitsInByte - 3);

		const bool isHorizontallyFlipped = globalTileIds[i] & flippedHorizontally;
		const bool isVerticallyFlipped = globalTileIds[i] & flippedVertically;
		const bool isDiagonallyFlipped = globalTileIds[i] & flippedDiagonally;

		const unsigned globalTileId = globalTileIds[i] & (~(flippedHorizontally | flippedVertically | flippedDiagonally));

		if (hasTile(globalTileId)) {
			const std::size_t tilesetIndex = findTilesetIndex(globalTileId, tilesets);
			if (tilesetIndex == std::string::npos) {
				PH_LOG(LogType::Warning, "It was not possible to find tileset for " + std::to_string(globalTileId));
				continue;
			}
			const unsigned tileId = globalTileId - tilesets.firstGlobalTileIds[tilesetIndex];
			sf::Vector2u tileRectPosition = Math::toTwoDimensional(tileId, tilesets.columnsCounts[tilesetIndex]);
			tileRectPosition.x *= tileSize.x;
			tileRectPosition.y *= tileSize.y;
			const sf::IntRect tileRect(
				static_cast<sf::Vector2i>(tileRectPosition),
				static_cast<sf::Vector2i>(tileSize)
			);
			const std::string textureName = pathToMapTextures + tilesets.sources[tilesetIndex];
			const sf::Texture& texture = mGameData->getTextures().get(textureName);
			sf::Sprite tile(texture, tileRect);
			sf::Vector2f position(Math::toTwoDimensional(i, mapSize.x));
			position.x *= tileSize.x;
			position.y *= tileSize.y;
			if (isHorizontallyFlipped || isVerticallyFlipped || isDiagonallyFlipped) {
				const sf::Vector2f center(tileSize.x / 2.f, tileSize.y / 2.f);
				tile.setOrigin(center);
				position += center;
				if (isHorizontallyFlipped && isVerticallyFlipped && isDiagonallyFlipped) {
					tile.setRotation(270);
					tile.setScale(1.f, -1.f);
				}
				else if (isHorizontallyFlipped && isVerticallyFlipped)
					tile.setScale(-1.f, -1.f);
				else if (isHorizontallyFlipped && isDiagonallyFlipped)
					tile.setRotation(90);
				else if (isHorizontallyFlipped)
					tile.setScale(-1.f, 1.f);
				else if (isVerticallyFlipped && isDiagonallyFlipped)
					tile.setRotation(270);
				else if (isVerticallyFlipped)
					tile.setScale(1.f, -1.f);
				else if (isDiagonallyFlipped) {
					tile.setRotation(270);
					tile.setScale(-1.f, 1.f);
				}
			}
			tile.setPosition(position);
			mTiles.push_back(tile);

			for (std::size_t j = 0; j < tilesets.tiles.size(); ++j) {
				if (tilesets.firstGlobalTileIds[tilesetIndex] == tilesets.tiles[j].firstGlobalTileId) { 
					for (std::size_t k = 0; k < tilesets.tiles[j].ids.size(); ++k) {
						if (tileId == tilesets.tiles[j].ids[k]) {
							sf::FloatRect bounds = tilesets.tiles[j].bounds[k];
							bounds.left += position.x;
							bounds.top += position.y;
							std::unique_ptr<CollisionBody> collisionBody =
								std::make_unique<CollisionBody>(bounds, 0, BodyType::staticBody, this, mGameData);
							mCollisionBodies.push_back(std::move(collisionBody));
						}
					}
					break; 
				}
			}
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
	auto& camera = mGameData->getRenderer().getCamera();
	const sf::Vector2f center = camera.getCenter();
	const sf::Vector2f size = camera.getSize();
	const sf::Vector2f tileSize(32, 32);
	const sf::Vector2f topLeftCornerPosition(center.x - size.x / 2, center.y - size.y / 2);
	sf::FloatRect screen(topLeftCornerPosition.x, topLeftCornerPosition.y, size.x, size.y);

	for (const sf::Sprite& sprite : mTiles) {
		const sf::FloatRect bounds = sprite.getGlobalBounds();
		screen.left -= bounds.width;
		screen.top -= bounds.height;
		screen.width += bounds.width;
		screen.height += bounds.height;
		if (screen.contains(bounds.left, bounds.top))
			target.draw(sprite, states);
	}
}
