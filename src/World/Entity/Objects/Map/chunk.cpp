#include "chunk.hpp"

namespace ph {

Chunk::Chunk(
	const sf::Vector2f topLeftCornerPositionInWorld,
	std::shared_ptr<ChunkData> chunkData
)
	:mTopLeftCornerPositionInWorld(topLeftCornerPositionInWorld)
	,mChunkData(chunkData)
{
	const sf::Vector2u chunkSizeInTiles = mChunkData->getChunkSizeInTiles();
	mTilesToCreate.reserve(chunkSizeInTiles.x * chunkSizeInTiles.y);
}

void Chunk::initializeGraphics()
{
	mVertexArray.setPrimitiveType(sf::Quads);

	const sf::Vector2u chunkSizeInTiles = mChunkData->getChunkSizeInTiles();
	mVertexArray.resize(chunkSizeInTiles.x * chunkSizeInTiles.y * 4);

	for(unsigned y = 0; y < mChunkData->getChunkSizeInTiles().y; ++y)
	{
		for(unsigned x = 0; x < mChunkData->getChunkSizeInTiles().x; ++x)
		{
			const unsigned int tileIdInChunk = y * chunkSizeInTiles.x + x;
			sf::Vertex* const tile = &mVertexArray[tileIdInChunk * 4];

			if(tileIdInChunk >= mTilesToCreate.size())
				break;

			const TileData& tileData = mTilesToCreate.at(tileIdInChunk);
			initializeTextureCoordinates(tileData, tile);
			initializeVertexPositions(tileData, tile);
		}
	}

	mTilesToCreate.clear();
}

void Chunk::initializeTextureCoordinates(const TileData& tileData, sf::Vertex* const tile) const
{
	const auto textureCoordinateIndices = getTextureCoordinateIndices(tileData);
	const sf::Vector2f textureRectTopLeftCorner = static_cast<sf::Vector2f>(tileData.mTextureRectTopLeftCorner);
	const sf::Vector2u tileSizeInPixels = mChunkData->getTileSizeInPixels();
	tile[textureCoordinateIndices[0]].texCoords = textureRectTopLeftCorner;
	tile[textureCoordinateIndices[1]].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y);
	tile[textureCoordinateIndices[2]].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
	tile[textureCoordinateIndices[3]].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
}

auto Chunk::getTextureCoordinateIndices(const TileData& tileData) const -> std::array<int, 4>
{
	const bool isHorizontallyFlipped = tileData.mFlipData.mIsHorizontallyFlipped;
	const bool isVerticallyFlipped = tileData.mFlipData.mIsVerticallyFlipped;
	const bool isDiagonallyFlipped = tileData.mFlipData.mIsDiagonallyFlipped;

	std::array<int, 4> textureIndices;

	if(!(isHorizontallyFlipped || isVerticallyFlipped || isDiagonallyFlipped))
		textureIndices = {0, 1, 2, 3};
	else if(isHorizontallyFlipped && isVerticallyFlipped && isDiagonallyFlipped)
		textureIndices = {2, 1, 0, 3};
	else if(isHorizontallyFlipped && isVerticallyFlipped)
		textureIndices = {2, 3, 0, 1};
	else if(isHorizontallyFlipped && isDiagonallyFlipped)
		textureIndices = {1, 2, 3, 0};
	else if(isVerticallyFlipped && isDiagonallyFlipped)
		textureIndices = {3, 0, 1, 2};
	else if(isHorizontallyFlipped)
		textureIndices = {1, 0, 3, 2};
	else if(isVerticallyFlipped)
		textureIndices = {3, 2, 1, 0};
	else if(isDiagonallyFlipped)
		textureIndices = {0, 3, 2, 1};

	return textureIndices;
}

void Chunk::initializeVertexPositions(const TileData& tileData, sf::Vertex* const tile) const
{
	const sf::Vector2u tileSizeInPixels = mChunkData->getTileSizeInPixels();
	const sf::Vector2f vertexTopLeftCornerPosition = tileData.mTopLeftCornerPositionInWorld;
	tile[0].position = vertexTopLeftCornerPosition;
	tile[1].position = sf::Vector2f(vertexTopLeftCornerPosition.x + tileSizeInPixels.x, vertexTopLeftCornerPosition.y);
	tile[2].position = sf::Vector2f(vertexTopLeftCornerPosition.x + tileSizeInPixels.x, vertexTopLeftCornerPosition.y + tileSizeInPixels.y);
	tile[3].position = sf::Vector2f(vertexTopLeftCornerPosition.x, vertexTopLeftCornerPosition.y + tileSizeInPixels.y);
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &mChunkData->getTileset();
	target.draw(mVertexArray, states);
}

sf::FloatRect Chunk::getGlobalBounds() const
{
	const sf::Vector2f chunkSizeInPixels = static_cast<sf::Vector2f>(mChunkData->getChunkSizeInPixels());
	return sf::FloatRect(mTopLeftCornerPositionInWorld.x, mTopLeftCornerPositionInWorld.y, chunkSizeInPixels.x, chunkSizeInPixels.y);
}

}