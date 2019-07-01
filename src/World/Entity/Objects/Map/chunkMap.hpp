#pragma once

#include "chunk.hpp"
#include <vector>

namespace ph {

class LayerOfChunks
{
public:
	explicit LayerOfChunks(const sf::Vector2u mapSizeInTiles, const sf::Vector2u tileSizeInPixels, const sf::Texture& tileset);
	bool doesMapNotFitInChunksOnXAxis(const sf::Vector2u mapSizeInTiles);
	bool doesMapNotFitInChunksOnYAxis(const sf::Vector2u mapSizeInTiles);

	void addTileData(const TileData&);
	sf::Vector2u getChunkPositionInVectorOfChunksToWhichNewTileShouldBeAdded(const TileData&);

	void initializeGraphics();

	void draw(sf::RenderTarget&, const sf::RenderStates, const sf::FloatRect&) const;
private:
	bool isChunkInCamera(const Chunk& chunk, const sf::FloatRect& cameraBounds) const;

private:
	using RowOfChunks = std::vector<Chunk>;
	using ChunkMatrix = std::vector<RowOfChunks>;
	ChunkMatrix mAllChunksInLayer;
	const sf::Vector2u mMapSizeInTiles;
	const sf::Vector2u mTileSizeInPixels;
};

class ChunkMap
{
public:
	explicit ChunkMap(const sf::Vector2u mapSizeInTiles, const sf::Vector2u tileSizeInPixels, const sf::Texture& tileset);

	void addNewLayerOfChunks() { mLayers.emplace_back(mMapSizeInTiles, mTileSizeInPixels, mTileset); };

	void addTileData(const TileData& tile) { mLayers.back().addTileData(tile); }

	void initializeGraphicsForCurrentLayer() { mLayers.back().initializeGraphics(); }

	void draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::FloatRect& cameraBounds) const;

private:
	std::vector<LayerOfChunks> mLayers;
	const sf::Vector2u mMapSizeInTiles;
	const sf::Vector2u mTileSizeInPixels;
	const sf::Texture& mTileset;
};

}