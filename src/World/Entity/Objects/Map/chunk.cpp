#include "chunk.hpp"
#include "mapConstants.hpp"
#include <iostream>

namespace ph {

Chunk::Chunk(
	const sf::Texture& tileset,
	const sf::Vector2f topLeftCornerPositionInWorld,
	const sf::Vector2u tileSizeInPixels
)
	:mTileset(tileset)
	,mTopLeftCornerPositionInWorld(topLeftCornerPositionInWorld)
	,mTileSizeInPixels(tileSizeInPixels)
{
	using namespace MapConstants;
	mTilesToCreate.reserve(chunkSizeInTiles.x * chunkSizeInTiles.y);
}

void Chunk::initializeGraphics()
{
	using namespace MapConstants;

	mVertexArray.setPrimitiveType(sf::Quads);
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

			const sf::Vector2f textureRectTopLeftCorner = static_cast<sf::Vector2f>(tileData.mTextureRectTopLeftCorner);
			if(tileData.mFlippingData.mIsHorizontallyFlipped || 
			   tileData.mFlippingData.mIsVerticallyFlipped ||
			   tileData.mFlippingData.mIsDiagonallyFlipped) 
			{
				if(tileData.mFlippingData.mIsHorizontallyFlipped) {
					std::cout << "horizontally flipped" << std::endl;
					tile[1].texCoords = textureRectTopLeftCorner;
					tile[0].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
					tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
				}
				else if(tileData.mFlippingData.mIsVerticallyFlipped) {
					std::cout << "vertically flipped" << std::endl;
					tile[3].texCoords = textureRectTopLeftCorner;
					tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
					tile[0].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
				}
				else if(tileData.mFlippingData.mIsDiagonallyFlipped) {
					std::cout << "diagonaly flipped" << std::endl;
					tile[0].texCoords = textureRectTopLeftCorner;
					tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
					tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
				}
				else if(tileData.mFlippingData.mIsHorizontallyFlipped && tileData.mFlippingData.mIsVerticallyFlipped) {
					std::cout << "horizontally and vertically flipped" << std::endl;
					//TODO: Here will be an error! Fix it! // Here is error in loading!
					tile[2].texCoords = textureRectTopLeftCorner;
					tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[0].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
					tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
				}
				else if(tileData.mFlippingData.mIsHorizontallyFlipped && tileData.mFlippingData.mIsDiagonallyFlipped) {
					std::cout << "horizontally and diagonally flipped" << std::endl;
					tile[1].texCoords = textureRectTopLeftCorner;
					tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
					tile[0].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
				}
				else if(tileData.mFlippingData.mIsVerticallyFlipped && tileData.mFlippingData.mIsDiagonallyFlipped) {
					std::cout << "vertically and diagonally flipped" << std::endl;
					tile[3].texCoords = textureRectTopLeftCorner;
					tile[0].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
					tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
				}
				else if(tileData.mFlippingData.mIsHorizontallyFlipped && tileData.mFlippingData.mIsVerticallyFlipped && tileData.mFlippingData.mIsDiagonallyFlipped) {
					std::cout << "vertically, horizontally and diagonally flipped" << std::endl;
					//TODO: Here will be an error! Fix it!
					tile[2].texCoords = textureRectTopLeftCorner;
					tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y);
					tile[0].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
					tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
				}
			}
			else {
				tile[0].texCoords = textureRectTopLeftCorner;
				tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y);
				tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
				tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
			}

			const sf::Vector2f vertexTopLeftCornerPosition = tileData.mTopLeftCornerPositionInWorld;
			tile[0].position = vertexTopLeftCornerPosition;
			tile[1].position = sf::Vector2f(vertexTopLeftCornerPosition.x + mTileSizeInPixels.x, vertexTopLeftCornerPosition.y);
			tile[2].position = sf::Vector2f(vertexTopLeftCornerPosition.x + mTileSizeInPixels.x, vertexTopLeftCornerPosition.y + mTileSizeInPixels.y);
			tile[3].position = sf::Vector2f(vertexTopLeftCornerPosition.x, vertexTopLeftCornerPosition.y + mTileSizeInPixels.y);
		}
	}

	mTilesToCreate.clear();
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &mTileset;
	target.draw(mVertexArray, states);
}

sf::FloatRect Chunk::getGlobalBounds() const
{
	using namespace MapConstants;
	const sf::Vector2f chunkSizeInPixels(chunkSizeInTiles.x * mTileSizeInPixels.x, chunkSizeInTiles.y * mTileSizeInPixels.y);
	return sf::FloatRect(mTopLeftCornerPositionInWorld.x, mTopLeftCornerPositionInWorld.y, chunkSizeInPixels.x, chunkSizeInPixels.y);
}

}