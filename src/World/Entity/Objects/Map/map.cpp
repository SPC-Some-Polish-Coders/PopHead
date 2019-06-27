#include "map.hpp"
#include "gameData.hpp"
#include "Utilities/debug.hpp"
#include "Utilities/csv.hpp"
#include "Utilities/math.hpp"

#include <array>

namespace ph {

Map::Map(GameData* gameData, std::string name)
	:Object(gameData, name, LayerID::floorEntities)
	,mChunks(nullptr)
{
}

void Map::loadFromFile(const std::string& filename)
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
	mChunks = std::make_unique<ChunkMap>(mapSize, mGameData->getTextures().get(pathToTileset));
	for (const Xml& layerNode : layerNodes) {
		const Xml dataNode = layerNode.getChild("data");
		const std::vector<unsigned> globalTileIds = toGlobalTileIds(dataNode);
		loadTiles(globalTileIds, tilesets, mapSize, tileSize);
	}
}

void Map::checkMapSupport(const Xml& mapNode) const
{
	const std::string orientation = mapNode.getAttribute("orientation").toString();
	if (orientation != "orthogonal")
		PH_EXCEPTION("Used unsupported map orientation: " + orientation);
	const std::string infinite = mapNode.getAttribute("infinite").toString();
	if (infinite != "0")
		PH_EXCEPTION("Infinite maps are not supported");
}

sf::Vector2u Map::getMapSize(const Xml& mapNode) const
{
	return sf::Vector2u(
		mapNode.getAttribute("width").toUnsigned(),
		mapNode.getAttribute("height").toUnsigned()
	);
}

sf::Vector2u Map::getTileSize(const Xml& mapNode) const
{
	return sf::Vector2u(
		mapNode.getAttribute("tilewidth").toUnsigned(),
		mapNode.getAttribute("tileheight").toUnsigned()
	);
}

std::vector<Xml> Map::getTilesetNodes(const Xml& mapNode) const
{
	const std::vector<Xml> tilesetNodes = mapNode.getChildren("tileset");
	if (tilesetNodes.size() == 0)
		PH_LOG(LogType::Warning, "Map doesn't have any tilesets");
	return tilesetNodes;
}

Map::TilesetsData Map::getTilesetsData(const std::vector<Xml>& tilesetNodes) const
{
	TilesetsData tilesets;
	tilesets.firstGlobalTileIds.reserve(tilesetNodes.size());
	tilesets.tileCounts.reserve(tilesetNodes.size());
	tilesets.columnsCounts.reserve(tilesetNodes.size());
	tilesets.sources.reserve(tilesetNodes.size()); // WARNING: Change it when tilesets based on collection of images would be allowed
	for (Xml tilesetNode : tilesetNodes) {
		const unsigned firstGlobalTileId = tilesetNode.getAttribute("firstgid").toUnsigned();
		tilesets.firstGlobalTileIds.push_back(firstGlobalTileId);
		if (tilesetNode.hasAttribute("source")) {
			std::string tilesetNodeSource = tilesetNode.getAttribute("source").toString();
			tilesetNodeSource = pathToMapNotEmbeddedTilesets + Path::toFilename(tilesetNodeSource, '/');
			PH_LOG(LogType::Info, "Detected not embeded tileset in Map: " + tilesetNodeSource);
			Xml tilesetDocument;
			tilesetDocument.loadFromFile(tilesetNodeSource);
			tilesetNode = tilesetDocument.getChild("tileset");
		}
		tilesets.tileCounts.push_back(tilesetNode.getAttribute("tilecount").toUnsigned());
		tilesets.columnsCounts.push_back(tilesetNode.getAttribute("columns").toUnsigned());
		const Xml imageNode = tilesetNode.getChild("image");
		std::string source = imageNode.getAttribute("source").toString();
		source = Path::toFilename(source, '/');
		tilesets.sources.push_back(source);
		const std::vector<Xml> tileNodes = tilesetNode.getChildren("tile");
		TilesetsData::TilesData tilesData = getTilesData(tileNodes);
		tilesData.firstGlobalTileId = firstGlobalTileId;
		tilesets.tilesData.push_back(tilesData);
	}
	return tilesets;
}

Map::TilesetsData::TilesData Map::getTilesData(const std::vector<Xml>& tileNodes) const
{
	TilesetsData::TilesData tilesData{};
	tilesData.ids.reserve(tileNodes.size());
	tilesData.bounds.reserve(tileNodes.size()); // WARNING: Change it when there would be possibility to have more than one CollisionBody per tile?
	for (const Xml& tileNode : tileNodes) {
		tilesData.ids.push_back(tileNode.getAttribute("id").toUnsigned());
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
		tilesData.bounds.push_back(bounds);
	}
	return tilesData;
}

std::vector<Xml> Map::getLayerNodes(const Xml& mapNode) const
{
	const std::vector<Xml> layerNodes = mapNode.getChildren("layer");
	if (layerNodes.size() == 0)
		PH_LOG(LogType::Warning, "Map doesn't have any layers");
	return layerNodes;
}

std::vector<unsigned> Map::toGlobalTileIds(const Xml& dataNode) const
{
	const std::string encoding = dataNode.getAttribute("encoding").toString();
	if (encoding == "csv")
		return Csv::toUnsigneds(dataNode.toString());
	PH_EXCEPTION("Used unsupported data encoding: " + encoding);
}

void Map::loadTiles(
	const std::vector<unsigned>& globalTileIds,
	const TilesetsData& tilesets,
	sf::Vector2u mapSize,
	sf::Vector2u tileSize)
{
	const sf::Texture& texture = mGameData->getTextures().get(pathToTileset);

	mChunks->addNewLayerOfChunks();

	for (std::size_t i = 0; i < globalTileIds.size(); ++i) {
		const unsigned bitsInByte = 8;
		const unsigned flippedHorizontally = 1u << (sizeof(unsigned) * bitsInByte - 1);
		const unsigned flippedVertically = 1u << (sizeof(unsigned) * bitsInByte - 2);
		const unsigned flippedDiagonally = 1u << (sizeof(unsigned) * bitsInByte - 3);

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

			sf::Vector2f position(Math::toTwoDimensional(i, mapSize.x));
			position.x *= tileSize.x;
			position.y *= tileSize.y;

			TileData tileData;
			tileData.mTextureRectTopLeftCorner = tileRectPosition;
			tileData.mTopLeftCornerPositionInWorld = position;

			mChunks->addTile(tileData);
			
			//if (isHorizontallyFlipped || isVerticallyFlipped || isDiagonallyFlipped) {
			//	const sf::Vector2f center(tileSize.x / 2.f, tileSize.y / 2.f);
			//	tile.setOrigin(center);
			//	position += center;
			//	if (isHorizontallyFlipped && isVerticallyFlipped && isDiagonallyFlipped) {
			//		tile.setRotation(270);
			//		tile.setScale(1.f, -1.f);
			//	}
			//	else if (isHorizontallyFlipped && isVerticallyFlipped)
			//		tile.setScale(-1.f, -1.f);
			//	else if (isHorizontallyFlipped && isDiagonallyFlipped)
			//		tile.setRotation(90);
			//	else if (isHorizontallyFlipped)
			//		tile.setScale(-1.f, 1.f);
			//	else if (isVerticallyFlipped && isDiagonallyFlipped)
			//		tile.setRotation(270);
			//	else if (isVerticallyFlipped)
			//		tile.setScale(1.f, -1.f);
			//	else if (isDiagonallyFlipped) {
			//		tile.setRotation(270);
			//		tile.setScale(-1.f, 1.f);
			//	}
			//}
			//tile.setPosition(position);
			//mTiles.push_back(tile);

			const std::size_t tilesDataIndex = findTilesIndex(tilesets.firstGlobalTileIds[tilesetIndex], tilesets.tilesData);
			if (tilesDataIndex == std::string::npos)
				continue;
			loadCollisionBodies(tileId, tilesets.tilesData[tilesDataIndex], position);
		}
	}

	mChunks->createGraphicsForCurrentLayer();
}

std::size_t Map::findTilesetIndex(unsigned globalTileId, const TilesetsData& tilesets) const
{
	for (std::size_t i = 0; i < tilesets.firstGlobalTileIds.size(); ++i) {
		const unsigned firstGlobalTileId = tilesets.firstGlobalTileIds[i];
		const unsigned lastGlobalTileId = firstGlobalTileId + tilesets.tileCounts[i] - 1;
		if (globalTileId >= firstGlobalTileId && globalTileId <= lastGlobalTileId)
			return i;
	}
	return std::string::npos;
}

std::size_t Map::findTilesIndex(unsigned firstGlobalTileId, const std::vector<TilesetsData::TilesData>& tilesData) const
{
	for (std::size_t i = 0; i < tilesData.size(); ++i)
		if (firstGlobalTileId == tilesData[i].firstGlobalTileId)
			return i;
	return std::string::npos;
}

void Map::loadCollisionBodies(
	unsigned tileId,
	const TilesetsData::TilesData& tilesData,
	sf::Vector2f position)
{
	for (std::size_t i = 0; i < tilesData.ids.size(); ++i) {
		if (tileId == tilesData.ids[i]) {
			sf::FloatRect bounds = tilesData.bounds[i];
			bounds.left += position.x;
			bounds.top += position.y;
			auto collisionBody = std::make_unique<CollisionBody>(bounds, 0, BodyType::staticBody, this, mGameData);
			mCollisionBodies.push_back(std::move(collisionBody));
		}
	}
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	mChunks->draw(target, states);

	//auto& camera = mGameData->getRenderer().getCamera();
	//const sf::Vector2f center = camera.getCenter();
	////const sf::Vector2f size = camera.getSize();
	//const sf::Vector2f size(16*40, 16*30);
	//const sf::Vector2f tileSize(16, 16);
	//const sf::Vector2f topLeftCornerPosition(center.x - size.x / 2, center.y - size.y / 2);
	//sf::FloatRect screen(topLeftCornerPosition.x, topLeftCornerPosition.y, size.x, size.y);

	//for (const sf::Sprite& sprite : mTiles) {
	//	const sf::FloatRect bounds = sprite.getGlobalBounds();
	//	screen.left -= bounds.width;
	//	screen.top -= bounds.height;
	//	screen.width += bounds.width;
	//	screen.height += bounds.height;
	//	if(screen.contains(bounds.left, bounds.top)) {
	//		target.draw(sprite, states);
	//		mGameData->getEfficencyRegister().registerRenderCall();
	//	}
	//}
}

}