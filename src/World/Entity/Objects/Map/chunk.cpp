#include "chunk.hpp"
#include "mapConstants.hpp"

namespace ph {

Chunk::Chunk(const sf::Texture& tileset)
	:mTileset(tileset)
{
	using namespace MapConstants;
	mTilesToCreate.reserve(mChunkSize.x * mChunkSize.y);
}

void Chunk::initializeGraphics()
{
	using namespace MapConstants;

	mVertexArray.setPrimitiveType(sf::Quads);
	mVertexArray.resize(mChunkSize.x * mChunkSize.y * 4);

	for(int y = 0; y < 24; ++y)
	{
		for(int x = 0; x < 24; ++x)
		{
			const unsigned int tileIdInChunk = y * mChunkSize.x + x;
			sf::Vertex* const tile = &mVertexArray[tileIdInChunk * 4];

			if(tileIdInChunk >= mTilesToCreate.size())
				break;

			const TileData& tileData = mTilesToCreate.at(tileIdInChunk);

			//TODO: Make rotaion and flipping possible
			const sf::Vector2f textureRectTopLeftCorner = static_cast<sf::Vector2f>(tileData.mTextureRectTopLeftCorner);
			tile[0].texCoords = textureRectTopLeftCorner;
			tile[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSize.x, textureRectTopLeftCorner.y);
			tile[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSize.x, textureRectTopLeftCorner.y + mTileSize.y);
			tile[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + mTileSize.y);

			const sf::Vector2f vertexTopLeftCornerPosition = tileData.mTopLeftCornerPositionInWorld;
			tile[0].position = vertexTopLeftCornerPosition;
			tile[1].position = sf::Vector2f(vertexTopLeftCornerPosition.x + mTileSize.x, vertexTopLeftCornerPosition.y);
			tile[2].position = sf::Vector2f(vertexTopLeftCornerPosition.x + mTileSize.x, vertexTopLeftCornerPosition.y + mTileSize.y);
			tile[3].position = sf::Vector2f(vertexTopLeftCornerPosition.x, vertexTopLeftCornerPosition.y + mTileSize.y);
		}
	}

	mTilesToCreate.clear();
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &mTileset;
	target.draw(mVertexArray, states);
}

}