#include "chunk.hpp"
#include "mapConstants.hpp"

namespace ph {

Chunk::Chunk(const sf::Texture& tileset, const sf::Vector2f topLeftCornerPositionInWorld)
	:mTileset(tileset)
	,mTopLeftCornerPositionInWorld(topLeftCornerPositionInWorld)
{
	using namespace MapConstants;
	mTilesToCreate.reserve(mChunkSizeInTiles.x * mChunkSizeInTiles.y);
}

void Chunk::initializeGraphics()
{
	using namespace MapConstants;

	mVertexArray.setPrimitiveType(sf::Quads);
	mVertexArray.resize(mChunkSizeInTiles.x * mChunkSizeInTiles.y * 4);

	for(int y = 0; y < 24; ++y)
	{
		for(int x = 0; x < 24; ++x)
		{
			const unsigned int tileIdInChunk = y * mChunkSizeInTiles.x + x;
			sf::Vertex* const tile = &mVertexArray[tileIdInChunk * 4];

			if(tileIdInChunk >= mTilesToCreate.size())
				break;

			const TileData& tileData = mTilesToCreate.at(tileIdInChunk);

			//TODO: Make rotaion and flipping possible
			const sf::Vector2f textureRectTopLeftCorner = static_cast<sf::Vector2f>(tileData.mTextureRectTopLeftCorner);
			tile[0].texCoords = textureRectTopLeftCorner;
			tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y);
			tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSizeInPixels.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);
			tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + mTileSizeInPixels.y);

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
	constexpr float chunkSide = 384;
	return sf::FloatRect(mTopLeftCornerPositionInWorld.x, mTopLeftCornerPositionInWorld.y, chunkSide, chunkSide);
}

}