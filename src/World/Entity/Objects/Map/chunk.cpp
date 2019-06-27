#include "chunk.hpp"

namespace ph {

Chunk::Chunk(const sf::Texture& tileset)
	:mTileset(tileset)
{
	mTilesToCreate.reserve(mChunkSize.x * mChunkSize.y );
}

void Chunk::create()
{
	mVertexArray.setPrimitiveType(sf::Quads);
	mVertexArray.resize(mChunkSize.x * mChunkSize.y * 4);

	for(int y = 0; y < 24; ++y)
	{
		for(int x = 0; x < 24; ++x)
		{
			const int tileIdInChunk = y * mChunkSize.x + x;
			sf::Vertex* const quad = &mVertexArray[tileIdInChunk * 4];

			//TODO: avail copying
			if(tileIdInChunk >= mTilesToCreate.size())
				break;

			Tile tile = mTilesToCreate[tileIdInChunk];

			//TODO: Make rotaion and flipping possible
			const sf::Vector2f textureRectTopLeftCorner = static_cast<sf::Vector2f>(tile.mTextureRectTopLeftCorner);
			quad[0].texCoords = textureRectTopLeftCorner;
			quad[1].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSize.x, textureRectTopLeftCorner.y);
			quad[2].texCoords = sf::Vector2f(textureRectTopLeftCorner.x + mTileSize.x, textureRectTopLeftCorner.y + mTileSize.y);
			quad[3].texCoords = sf::Vector2f(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y + mTileSize.y);

			const sf::Vector2f vertexTopLeftCornerPosition = tile.mTopLeftCornerPositionInWorld;
			quad[0].position = vertexTopLeftCornerPosition;
			quad[1].position = sf::Vector2f(vertexTopLeftCornerPosition.x + mTileSize.x, vertexTopLeftCornerPosition.y);
			quad[2].position = sf::Vector2f(vertexTopLeftCornerPosition.x + mTileSize.x, vertexTopLeftCornerPosition.y + mTileSize.y);
			quad[3].position = sf::Vector2f(vertexTopLeftCornerPosition.x, vertexTopLeftCornerPosition.y + mTileSize.y);
		}
	}

	mTilesToCreate.clear();
}

void Chunk::addAdditionalEmptyTile()
{
	/*Tile tile;
	tile.
	addTile(std::move(tile));*/
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &mTileset;
	target.draw(mVertexArray, states);
}

}