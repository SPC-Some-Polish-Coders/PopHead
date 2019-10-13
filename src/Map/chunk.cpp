#include "chunk.hpp"
#include "Utilities/rect.hpp"

namespace ph {

Chunk::Chunk(const sf::Vector2f topLeftCornerPositionInWorld, std::shared_ptr<ChunkData> chunkData)
	:mTopLeftCornerPositionInWorld(topLeftCornerPositionInWorld)
	,mChunkData(chunkData)
	,mVertexArray(std::make_shared<VertexArray>())
{
	const sf::Vector2u chunkSizeInTiles = mChunkData->getChunkSizeInTiles();
	mTilesToCreate.reserve(chunkSizeInTiles.x * chunkSizeInTiles.y);
}

void Chunk::addTileData(const TileData& tile)
{
	mTilesToCreate.emplace_back(tile);
}

void Chunk::initializeGraphics()
{
	// create vector with data for vertex buffer
	std::vector<float> vertices;
	const sf::Vector2u chunkSizeInTiles = mChunkData->getChunkSizeInTiles();
	constexpr unsigned verticesPerTile = 4u;
	constexpr unsigned nrOfFloatsInVertex = 4u;
	vertices.reserve(chunkSizeInTiles.x * chunkSizeInTiles.y * verticesPerTile * nrOfFloatsInVertex);

	for(unsigned y = 0; y < chunkSizeInTiles.y; ++y)
	{
		for(unsigned x = 0; x < chunkSizeInTiles.x; ++x)
		{
			const unsigned int tileIdInChunk = y * chunkSizeInTiles.x + x;
			if(tileIdInChunk >= mTilesToCreate.size())
				break;
			
			const TileData& tileData = mTilesToCreate.at(tileIdInChunk);

			// tile vertex positions
			const sf::Vector2f tileSizeInPixels = static_cast<sf::Vector2f>(mChunkData->getTileSizeInPixels());
			const sf::Vector2f vertexTopLeftCornerPosition = tileData.mTopLeftCornerPositionInWorld;
			const FloatRect tileBounds(vertexTopLeftCornerPosition.x, vertexTopLeftCornerPosition.y, tileSizeInPixels.x, tileSizeInPixels.y);
			
			// TODO: Allow tile rotation and flipping

			// tile texture rect positions
			const sf::Vector2f textureRectTopLeftCorner = static_cast<sf::Vector2f>(tileData.mTextureRectTopLeftCorner);
			const sf::Vector2f textureSize = static_cast<sf::Vector2f>(mChunkData->getTileset().getSize());
			const FloatRect textureRect(
				textureRectTopLeftCorner.x / textureSize.x,
				(textureSize.y - textureRectTopLeftCorner.y) / textureSize.y,
				tileSizeInPixels.x / textureSize.x,
				tileSizeInPixels.y / textureSize.y
			);

			// emplace back top left corner
			vertices.emplace_back(vertexTopLeftCornerPosition.x);
			vertices.emplace_back(vertexTopLeftCornerPosition.y);
			vertices.emplace_back(textureRect.left);
			vertices.emplace_back(textureRect.top);

			// emplace back top right corner
			const sf::Vector2f topRightPositions = tileBounds.getTopRight();
			vertices.emplace_back(topRightPositions.x);
			vertices.emplace_back(topRightPositions.y);
			const sf::Vector2f topRightTextureCoordinate = textureRect.getTopRight();
			vertices.emplace_back(topRightTextureCoordinate.x);
			vertices.emplace_back(topRightTextureCoordinate.y);

			// emplace back bottom right corner
			const sf::Vector2f bottomRightPositions = tileBounds.getBottomRight();
			vertices.emplace_back(bottomRightPositions.x);
			vertices.emplace_back(bottomRightPositions.y);
			vertices.emplace_back(textureRect.right());
			vertices.emplace_back(textureRect.top - textureRect.height);

			// emplace back bottom left corner
			const sf::Vector2f bottomLeftPositions = tileBounds.getBottomLeft();
			vertices.emplace_back(bottomLeftPositions.x);
			vertices.emplace_back(bottomLeftPositions.y);
			vertices.emplace_back(textureRect.left);
			vertices.emplace_back(textureRect.top - textureRect.height);
		}
	}

	VertexBuffer vbo = createVertexBuffer();
	setData(vbo, vertices.data(), vertices.size() * sizeof(float), DataUsage::staticDraw);
	mVertexArray->setVertexBuffer(vbo, VertexBufferLayout::position2_texCoords2);

	// create vector with data for index buffer
	std::vector<unsigned> indices;
	constexpr unsigned numberOfIndicesPerTile = 6;
	indices.resize(chunkSizeInTiles.x * chunkSizeInTiles.y * numberOfIndicesPerTile);

	unsigned nrOfTile = 0;
	for(unsigned nrOfIndexInTile = 0;;)
	{
		unsigned index = 0;
		switch(nrOfIndexInTile)
		{
			case 0: index = 1; break;
			case 1: index = 2; break;
			case 2: index = 0; break;
			case 3: index = 2; break;
			case 4: index = 3; break;
			case 5: index = 0; break;
			default:
				break;
		}
		unsigned nrOfIndexInChunk = nrOfTile * 6 + nrOfIndexInTile;
		indices[nrOfIndexInChunk] = index + nrOfTile * 4;
		
		if(nrOfIndexInTile == 5) {
			++nrOfTile;
			nrOfIndexInTile = 0;
		}
		else
			++nrOfIndexInTile;

		if(nrOfIndexInChunk + 2 == indices.size())
			break;
	}

	IndexBuffer ibo = createIndexBuffer();
	setData(ibo, indices.data(), indices.size());
	mVertexArray->setIndexBuffer(ibo);

	//new
	//1, 2, 0, 2, 3, 0

	//old
	//0, 1, 3, // first rectangle
	//1, 2, 3 // second rectangle
}

void Chunk::draw() const
{
	Renderer::submit(*mVertexArray, sf::Transform::Identity, static_cast<sf::Vector2i>(mChunkData->getChunkSizeInPixels()));
}

sf::FloatRect Chunk::getGlobalBounds() const
{
	const sf::Vector2f chunkSizeInPixels = static_cast<sf::Vector2f>(mChunkData->getChunkSizeInPixels());
	return sf::FloatRect(mTopLeftCornerPositionInWorld.x, mTopLeftCornerPositionInWorld.y, chunkSizeInPixels.x, chunkSizeInPixels.y);
}

}

// OLD SFML IMPLEMENTATION:

//auto Chunk::getTextureCoordinateIndices(const TileData& tileData) const -> std::array<int, 4>
//{
//	const bool isHorizontallyFlipped = tileData.mFlipData.mIsHorizontallyFlipped;
//	const bool isVerticallyFlipped = tileData.mFlipData.mIsVerticallyFlipped;
//	const bool isDiagonallyFlipped = tileData.mFlipData.mIsDiagonallyFlipped;
//
//	std::array<int, 4> textureIndices;
//
//	if(!(isHorizontallyFlipped || isVerticallyFlipped || isDiagonallyFlipped))
//		textureIndices = {0, 1, 2, 3};
//	else if(isHorizontallyFlipped && isVerticallyFlipped && isDiagonallyFlipped)
//		textureIndices = {2, 1, 0, 3};
//	else if(isHorizontallyFlipped && isVerticallyFlipped)
//		textureIndices = {2, 3, 0, 1};
//	else if(isHorizontallyFlipped && isDiagonallyFlipped)
//		textureIndices = {1, 2, 3, 0};
//	else if(isVerticallyFlipped && isDiagonallyFlipped)
//		textureIndices = {3, 0, 1, 2};
//	else if(isHorizontallyFlipped)
//		textureIndices = {1, 0, 3, 2};
//	else if(isVerticallyFlipped)
//		textureIndices = {3, 2, 1, 0};
//	else if(isDiagonallyFlipped)
//		textureIndices = {0, 3, 2, 1};
//
//	return textureIndices;
//}
//void Chunk::initializeGraphics()
//{
//	mVertexArray.setPrimitiveType(sf::Quads);
//
//	const sf::Vector2u chunkSizeInTiles = mChunkData->getChunkSizeInTiles();
//	mVertexArray.resize(chunkSizeInTiles.x * chunkSizeInTiles.y * 4);
//
//	for(unsigned y = 0; y < mChunkData->getChunkSizeInTiles().y; ++y)
//	{
//		for(unsigned x = 0; x < mChunkData->getChunkSizeInTiles().x; ++x)
//		{
//			const unsigned int tileIdInChunk = y * chunkSizeInTiles.x + x;
//			sf::Vertex* const tile = &mVertexArray[tileIdInChunk * 4];
//
//			if(tileIdInChunk >= mTilesToCreate.size())
//				break;
//
//			const TileData& tileData = mTilesToCreate.at(tileIdInChunk);
//			initializeTextureCoordinates(tileData, tile);
//			initializeVertexPositions(tileData, tile);
//		}
//	}
//
//	mTilesToCreate.clear();
//}
//
//void Chunk::initializeTextureCoordinates(const TileData& tileData, sf::Vertex* const tile) const
//{
//	const auto textureCoordinateIndices = getTextureCoordinateIndices(tileData);
//	const sf::Vector2f textureRectTopLeftCorner = static_cast<sf::Vector2f>(tileData.mTextureRectTopLeftCorner);
//	const auto tileSizeInPixels = static_cast<sf::Vector2f>(mChunkData->getTileSizeInPixels());
//	const FloatRect textureRect(textureRectTopLeftCorner.x, textureRectTopLeftCorner.y, tileSizeInPixels.x, tileSizeInPixels.y);
//	tile[textureCoordinateIndices[0]].texCoords = textureRectTopLeftCorner;
//	tile[textureCoordinateIndices[1]].texCoords = textureRect.getTopRight();
//	tile[textureCoordinateIndices[2]].texCoords = textureRect.getBottomRight();
//	tile[textureCoordinateIndices[3]].texCoords = textureRect.getBottomLeft();
//}
//
//
//void Chunk::initializeVertexPositions(const TileData& tileData, sf::Vertex* const tile) const
//{
//	const sf::Vector2f tileSizeInPixels = static_cast<sf::Vector2f>(mChunkData->getTileSizeInPixels());
//	const sf::Vector2f vertexTopLeftCornerPosition = tileData.mTopLeftCornerPositionInWorld;
//	const FloatRect tileBounds(vertexTopLeftCornerPosition.x, vertexTopLeftCornerPosition.y, tileSizeInPixels.x, tileSizeInPixels.y);
//	tile[0].position = vertexTopLeftCornerPosition;
//	tile[1].position = tileBounds.getTopRight();
//	tile[2].position = tileBounds.getBottomRight();
//	tile[3].position = tileBounds.getBottomLeft();
//}
//
//void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const
//{
//	states.texture = &mChunkData->getTileset();
//	target.draw(mVertexArray, states);
//}
//

