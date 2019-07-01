#include "chunk.hpp"
#include <iostream>

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

	for(int y = 0; y < 24; ++y)
	{
		for(int x = 0; x < 24; ++x)
		{
			const unsigned int tileIdInChunk = y * chunkSizeInTiles.x + x;
			sf::Vertex* const tile = &mVertexArray[tileIdInChunk * 4];

			if(tileIdInChunk >= mTilesToCreate.size())
				break;

			const TileData& tileData = mTilesToCreate.at(tileIdInChunk);

			const sf::Vector2u tileSizeInPixels = mChunkData->getTileSizeInPixels();

			const sf::Vector2f textureRectTopLeftCorner = static_cast<sf::Vector2f>(tileData.mTextureRectTopLeftCorner);
			if(tileData.mFlipData.mIsHorizontallyFlipped || 
			   tileData.mFlipData.mIsVerticallyFlipped ||
			   tileData.mFlipData.mIsDiagonallyFlipped) 
			{
				if(tileData.mFlipData.mIsHorizontallyFlipped) {
					tile[1].texCoords = textureRectTopLeftCorner;
					tile[0].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
					tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
				}
				else if(tileData.mFlipData.mIsVerticallyFlipped) {
					tile[3].texCoords = textureRectTopLeftCorner;
					tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
					tile[0].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
				}
				else if(tileData.mFlipData.mIsDiagonallyFlipped) {
					tile[0].texCoords = textureRectTopLeftCorner;
					tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
					tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
				}
				else if(tileData.mFlipData.mIsHorizontallyFlipped && tileData.mFlipData.mIsVerticallyFlipped) {
					//TODO: Here will be an error! Fix it! // Here is error in loading!
					tile[2].texCoords = textureRectTopLeftCorner;
					tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[0].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
					tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
				}
				else if(tileData.mFlipData.mIsHorizontallyFlipped && tileData.mFlipData.mIsDiagonallyFlipped) {
					tile[1].texCoords = textureRectTopLeftCorner;
					tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
					tile[0].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
				}
				else if(tileData.mFlipData.mIsVerticallyFlipped && tileData.mFlipData.mIsDiagonallyFlipped) {
					tile[3].texCoords = textureRectTopLeftCorner;
					tile[0].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
					tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
				}
				else if(tileData.mFlipData.mIsHorizontallyFlipped && tileData.mFlipData.mIsVerticallyFlipped && tileData.mFlipData.mIsDiagonallyFlipped) {
					//TODO: Here will be an error! Fix it!
					tile[2].texCoords = textureRectTopLeftCorner;
					tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[0].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
					tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
				}
			}
			else {
				tile[0].texCoords = textureRectTopLeftCorner;
				tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y);
				tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + tileSizeInPixels.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
				tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + tileSizeInPixels.y);
			}

			const sf::Vector2f vertexTopLeftCornerPosition = tileData.mTopLeftCornerPositionInWorld;
			tile[0].position = vertexTopLeftCornerPosition;
			tile[1].position = sf::Vector2f(vertexTopLeftCornerPosition.x + tileSizeInPixels.x, vertexTopLeftCornerPosition.y);
			tile[2].position = sf::Vector2f(vertexTopLeftCornerPosition.x + tileSizeInPixels.x, vertexTopLeftCornerPosition.y + tileSizeInPixels.y);
			tile[3].position = sf::Vector2f(vertexTopLeftCornerPosition.x, vertexTopLeftCornerPosition.y + tileSizeInPixels.y);
		}
	}

	mTilesToCreate.clear();
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