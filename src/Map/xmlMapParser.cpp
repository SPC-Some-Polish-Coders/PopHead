#include "xmlMapParser.hpp"
#include "Logs/logs.hpp"

#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

#include "AI/aiManager.hpp"
#include "Utilities/xml.hpp"
#include "Utilities/csv.hpp"
#include "Utilities/filePath.hpp"
#include "Utilities/math.hpp"

namespace ph {

void XmlMapParser::parseFile(const std::string& fileName, AIManager& aiManager, entt::registry& gameRegistry, EntitiesTemplateStorage& templates, TextureHolder& textures)
{
	PH_LOG_INFO("Map file (" + fileName + ") is being parsed.");

	mGameRegistry = &gameRegistry;
	mTemplates = &templates;
	mTextures = &textures;
		
	Xml mapFile;
	mapFile.loadFromFile(fileName);
	const Xml mapNode = mapFile.getChild("map");
	checkMapSupport(mapNode);

	GeneralMapInfo generalMapInfo = getGeneralMapInfo(mapNode);
	aiManager.registerMapSize(generalMapInfo.mapSize);

	const std::vector<Xml> tilesetNodes = getTilesetNodes(mapNode);
	const TilesetsData tilesetsData = getTilesetsData(tilesetNodes);
	const std::vector<Xml> layerNodes = getLayerNodes(mapNode);
	
	parserMapLayers(layerNodes, tilesetsData, generalMapInfo);
	createMapBorders(generalMapInfo);
}

void XmlMapParser::checkMapSupport(const Xml& mapNode) const
{
	const std::string orientation = mapNode.getAttribute("orientation").toString();
	if(orientation != "orthogonal")
		PH_EXCEPTION("Used unsupported map orientation: " + orientation);
	const std::string infinite = mapNode.getAttribute("infinite").toString();
	if(infinite != "0")
		PH_EXCEPTION("Infinite maps are not supported");
}

auto XmlMapParser::getGeneralMapInfo(const Xml& mapNode) const -> GeneralMapInfo
{
	return { getMapSize(mapNode), getTileSize(mapNode) };
}

sf::Vector2u XmlMapParser::getMapSize(const Xml& mapNode) const
{
	return sf::Vector2u(
		mapNode.getAttribute("width").toUnsigned(),
		mapNode.getAttribute("height").toUnsigned()
	);
}

sf::Vector2u XmlMapParser::getTileSize(const Xml& mapNode) const
{
	return sf::Vector2u(
		mapNode.getAttribute("tilewidth").toUnsigned(),
		mapNode.getAttribute("tileheight").toUnsigned()
	);
}

std::vector<Xml> XmlMapParser::getTilesetNodes(const Xml& mapNode) const
{
	const std::vector<Xml> tilesetNodes = mapNode.getChildren("tileset");
	if(tilesetNodes.size() == 0)
		PH_LOG_WARNING("Map doesn't have any tilesets");
	return tilesetNodes;
}

auto XmlMapParser::getTilesetsData(const std::vector<Xml>& tilesetNodes) const -> const TilesetsData
{
	TilesetsData tilesets;
	tilesets.firstGlobalTileIds.reserve(tilesetNodes.size());
	tilesets.tileCounts.reserve(tilesetNodes.size());
	tilesets.columnsCounts.reserve(tilesetNodes.size());
	
	for(Xml tilesetNode : tilesetNodes) {
		const unsigned firstGlobalTileId = tilesetNode.getAttribute("firstgid").toUnsigned();
		tilesets.firstGlobalTileIds.push_back(firstGlobalTileId);
		if(tilesetNode.hasAttribute("source")) {
			std::string tilesetNodeSource = tilesetNode.getAttribute("source").toString();
			tilesetNodeSource = FilePath::toFilename(tilesetNodeSource, '/');
			PH_LOG_INFO("Detected not embedded tileset in Map: " + tilesetNodeSource);
			Xml tilesetDocument;
			tilesetDocument.loadFromFile(tilesetNodeSource);
			tilesetNode = tilesetDocument.getChild("tileset");
		}

		tilesets.tileCounts.push_back(tilesetNode.getAttribute("tilecount").toUnsigned());
		tilesets.columnsCounts.push_back(tilesetNode.getAttribute("columns").toUnsigned());
		const Xml imageNode = tilesetNode.getChild("image");
		tilesets.tilesetFileName = FilePath::toFilename(imageNode.getAttribute("source").toString(), '/');
		const std::vector<Xml> tileNodes = tilesetNode.getChildren("tile");
		TilesData tilesData = getTilesData(tileNodes);
		tilesData.firstGlobalTileId = firstGlobalTileId;
		tilesets.tilesData.push_back(tilesData);
	}

	return tilesets;
}

auto XmlMapParser::getTilesData(const std::vector<Xml>& tileNodes) const -> TilesData
{
	TilesData tilesData{};
	tilesData.ids.reserve(tileNodes.size());
	tilesData.bounds.reserve(tileNodes.size());
	for(const Xml& tileNode : tileNodes) {
		tilesData.ids.push_back(tileNode.getAttribute("id").toUnsigned());
		const Xml objectGroupNode = tileNode.getChild("objectgroup");
		const Xml objectNode = objectGroupNode.getChild("object");
		const sf::FloatRect bounds(
			objectNode.getAttribute("x").toFloat(),
			objectNode.getAttribute("y").toFloat(),
			objectNode.hasAttribute("width") ? objectNode.getAttribute("width").toFloat() : 0.f,
			objectNode.hasAttribute("height") ? objectNode.getAttribute("height").toFloat() : 0.f
		);
		
		tilesData.bounds.push_back(bounds);
	}
	return tilesData;
}

std::vector<Xml> XmlMapParser::getLayerNodes(const Xml& mapNode) const
{
	const std::vector<Xml> layerNodes = mapNode.getChildren("layer");
	if(layerNodes.size() == 0)
		PH_LOG_WARNING("Map doesn't have any layers");
	return layerNodes;
}

void XmlMapParser::parserMapLayers(const std::vector<Xml>& layerNodes, const TilesetsData& tilesets, const GeneralMapInfo& info)
{
	unsigned char z = 200;
	for (const Xml& layerNode : layerNodes)
	{
		const Xml dataNode = layerNode.getChild("data");
		const auto globalIds = toGlobalTileIds(dataNode);
		createLayer(globalIds, tilesets, info, z);
		--z;
	}
}

std::vector<unsigned> XmlMapParser::toGlobalTileIds(const Xml& dataNode) const
{
	const std::string encoding = dataNode.getAttribute("encoding").toString();
	if(encoding == "csv")
		return Csv::toUnsigneds(dataNode.toString());
	PH_EXCEPTION("Used unsupported data encoding: " + encoding);
}

void XmlMapParser::createLayer(const std::vector<unsigned>& globalTileIds, const TilesetsData& tilesets,
                               const GeneralMapInfo& info, unsigned char z)
{
	constexpr float chunkSize = 12.f;
	float nrOfChunksInOneRow = std::ceil(info.mapSize.x / chunkSize);
	if(nrOfChunksInOneRow == 0.f)
		return;
	float nrOfChunksInOneColumn = std::ceil(info.mapSize.y / chunkSize);
	float nrOfChunks = nrOfChunksInOneRow * nrOfChunksInOneColumn;

	// create chunks
	std::vector<component::RenderChunk> chunks;
	chunks.resize(static_cast<size_t>(nrOfChunks));

	// fill chunks with z and bounds
	float rowSize = nrOfChunksInOneRow * chunkSize;
	for(size_t i = 0; i < chunks.size(); ++i)
	{
		chunks[i].z = z;

		float row = std::floor(i / nrOfChunksInOneRow);
		chunks[i].bounds = sf::FloatRect((chunkSize * i) - (row * chunkSize * nrOfChunksInOneRow), row * chunkSize, chunkSize, chunkSize);
	}

	for (size_t tileIndexInMap = 0; tileIndexInMap < globalTileIds.size(); ++tileIndexInMap) 
	{
		constexpr unsigned bitsInByte = 8;
		const unsigned flippedHorizontally = 1u << (sizeof(unsigned) * bitsInByte - 1);
		const unsigned flippedVertically = 1u << (sizeof(unsigned) * bitsInByte - 2);
		const unsigned flippedDiagonally = 1u << (sizeof(unsigned) * bitsInByte - 3);

		const bool isHorizontallyFlipped = globalTileIds[tileIndexInMap] & flippedHorizontally;
		const bool isVerticallyFlipped = globalTileIds[tileIndexInMap] & flippedVertically;
		const bool isDiagonallyFlipped = globalTileIds[tileIndexInMap] & flippedDiagonally;

		const unsigned globalTileId = globalTileIds[tileIndexInMap] & (~(flippedHorizontally | flippedVertically | flippedDiagonally));

		if (hasTile(globalTileId)) {
			const std::size_t tilesetIndex = findTilesetIndex(globalTileId, tilesets);
			if (tilesetIndex == std::string::npos) {
				PH_LOG_WARNING("It was not possible to find tileset for " + std::to_string(globalTileId));
				continue;
			}

			sf::Vector2f positionInTiles(Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(tileIndexInMap, info.mapSize.x));

			// create quad data
			QuadData qd;

			qd.position = sf::Vector2f(
				positionInTiles.x * static_cast<float>(info.tileSize.x),
				positionInTiles.y * static_cast<float>(info.tileSize.y));

			auto tileSize = static_cast<sf::Vector2f>(info.tileSize);
			qd.rotationOrigin = {tileSize.x / 2.f, tileSize.y / 2.f};
			if(!(isHorizontallyFlipped || isVerticallyFlipped || isDiagonallyFlipped)) {
				qd.size = tileSize;
				qd.rotation = 0.f;
			}
			else if(isHorizontallyFlipped && isVerticallyFlipped && isDiagonallyFlipped) {
				qd.size = {tileSize.x, -tileSize.y};
				qd.position.x += tileSize.x;
				qd.rotation = 270.f;
			}
			else if(isHorizontallyFlipped && isVerticallyFlipped) {
				qd.size = -tileSize;
				qd.position += tileSize;
				qd.rotation = 0.f;
			}
			else if(isHorizontallyFlipped && isDiagonallyFlipped) {
				qd.size = tileSize;
				qd.rotation = 90.f;
			}
			else if(isVerticallyFlipped && isDiagonallyFlipped) {
				qd.size = tileSize;
				qd.rotation = 270.f;
			}
			else if(isHorizontallyFlipped) {
				qd.size = {-tileSize.x, tileSize.y};
				qd.position.x += tileSize.x;
				qd.rotation = 0.f;
			}
			else if(isVerticallyFlipped) {
				qd.size = {tileSize.x, -tileSize.y};
				qd.position.y += tileSize.y;
				qd.rotation = 0.f;
			}
			else if(isDiagonallyFlipped) {
				qd.size = {-tileSize.x, tileSize.y};
				qd.position.y -= tileSize.x;
				qd.rotation = 270.f;
			}
			qd.rotation = Math::degreesToRadians(qd.rotation);

			qd.color = Vector4f{1.f, 1.f, 1.f, 1.f};
			qd.textureSlotRef = 0.f;

			const unsigned tileId = globalTileId - tilesets.firstGlobalTileIds[tilesetIndex];
			auto tileRectPosition = static_cast<sf::Vector2f>(
				Math::getTwoDimensionalPositionFromOneDimensionalArrayIndex(tileId, tilesets.columnsCounts[tilesetIndex]));
			tileRectPosition.x *= (info.tileSize.x + 2);
			tileRectPosition.y *= (info.tileSize.y + 2);
			tileRectPosition.x += 1;
			tileRectPosition.y += 1;
			const sf::Vector2f textureSize(576.f, 576.f); // TODO: Make it not hardcoded like that
			qd.textureRect.left = tileRectPosition.x / textureSize.x;
			qd.textureRect.top = (textureSize.y - tileRectPosition.y - info.tileSize.y) / textureSize.y;
			qd.textureRect.width = static_cast<float>(info.tileSize.x) / textureSize.x;
			qd.textureRect.height = static_cast<float>(info.tileSize.y) / textureSize.y;

			// TODO: Optimize that
			// emplace quad data to chunk
			for(auto& chunk : chunks)
				if(chunk.bounds.contains(positionInTiles))
					chunk.quads.emplace_back(qd);

			const std::size_t tilesDataIndex = findTilesIndex(tilesets.firstGlobalTileIds[tilesetIndex], tilesets.tilesData);
			if (tilesDataIndex == std::string::npos)
				continue;
			//loadCollisionBodies(tileId, tilesets.tilesData[tilesDataIndex], position);
		}
	}

	for(auto& chunk : chunks)
	{
		// transform chunk bounds to world coords so we can later use them for culling in RenderSystem
		chunk.bounds.left *= static_cast<float>(info.tileSize.x);
		chunk.bounds.top *= static_cast<float>(info.tileSize.y);
		chunk.bounds.width *= static_cast<float>(info.tileSize.x);
		chunk.bounds.height *=  static_cast<float>(info.tileSize.y);
		
		// add chunk entity which contains only RenderChunk component to registry
		auto chunkEntity = mTemplates->createCopy("RenderChunk", *mGameRegistry);
		auto& renderChunk = mGameRegistry->get<component::RenderChunk>(chunkEntity);
		renderChunk = chunk;
	}
}

bool XmlMapParser::hasTile(unsigned globalTileId) const
{
	return globalTileId != 0;
}

std::size_t XmlMapParser::findTilesetIndex(const unsigned globalTileId, const TilesetsData& tilesets) const
{
	for (std::size_t i = 0; i < tilesets.firstGlobalTileIds.size(); ++i) {
		const unsigned firstGlobalTileId = tilesets.firstGlobalTileIds[i];
		const unsigned lastGlobalTileId = firstGlobalTileId + tilesets.tileCounts[i] - 1;
		if (globalTileId >= firstGlobalTileId && globalTileId <= lastGlobalTileId)
			return i;
	}
	return std::string::npos;
}

std::size_t XmlMapParser::findTilesIndex(const unsigned firstGlobalTileId, const std::vector<TilesData>& tilesData) const
{
	for (std::size_t i = 0; i < tilesData.size(); ++i)
		if (firstGlobalTileId == tilesData[i].firstGlobalTileId)
			return i;
	return std::string::npos;
}

void XmlMapParser::loadCollisionBodies(const unsigned tileId, const TilesData& tilesData, const sf::Vector2f position)
{
	for (std::size_t i = 0; i < tilesData.ids.size(); ++i) {
		if (tileId == tilesData.ids[i]) {
			sf::FloatRect bounds = tilesData.bounds[i];
			bounds.left += position.x;
			bounds.top += position.y;
			//mGameData->getPhysicsEngine().createStaticBodyAndGetTheReference(bounds);   // TUTAJ
			//mGameData->getAIManager().registerObstacle({ bounds.left, bounds.top });   // TUTAJ
		}
	}
}

void XmlMapParser::createMapBorders(const GeneralMapInfo& mapInfo)
{
	// TUTAJ

	auto mapWidth = static_cast<float>(mapInfo.mapSize.x * mapInfo.tileSize.x);
	auto mapHeight = static_cast<float>(mapInfo.mapSize.y * mapInfo.tileSize.y);

	const sf::Vector2f size(sf::Vector2u(mapInfo.tileSize.x, mapInfo.tileSize.y));

	//auto& physics = mGameData->getPhysicsEngine();

	for (int x = -1; x < static_cast<int>(mapInfo.mapSize.x + 1); ++x)
	{
		// top border
		sf::Vector2f positionTop(x * size.x, -size.y);
		//physics.createStaticBodyAndGetTheReference({ positionTop, size });
		// bottom border
		sf::Vector2f positionBottom(x * size.x, mapHeight);
		//physics.createStaticBodyAndGetTheReference({ positionBottom, size });
	}

	for (int y = 0; y < static_cast<int>(mapInfo.mapSize.y); ++y)
	{
		// left border
		sf::Vector2f positionLeft(-size.x, y * size.y);
		//physics.createStaticBodyAndGetTheReference({ positionLeft, size });
		// right border
		sf::Vector2f positionRight(mapWidth, y * size.y);
		//physics.createStaticBodyAndGetTheReference({ positionRight, size });
	}
}

}
