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
	// create vertex buffer data
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

			// emplace back top left corner
			vertices.emplace_back(vertexTopLeftCornerPosition.x);
			vertices.emplace_back(vertexTopLeftCornerPosition.y);
			const sf::Vector2f topLeftCornerTextureCoordinates = getTextureCoordinate(tileData, 0);
			vertices.emplace_back(topLeftCornerTextureCoordinates.x);
			vertices.emplace_back(topLeftCornerTextureCoordinates.y);

			// emplace back top right corner
			const sf::Vector2f topRightPositions = tileBounds.getTopRight();
			vertices.emplace_back(topRightPositions.x);
			vertices.emplace_back(topRightPositions.y);
			const sf::Vector2f topRightCornerTextureCoordinates = getTextureCoordinate(tileData, 1);
			vertices.emplace_back(topRightCornerTextureCoordinates.x);
			vertices.emplace_back(topRightCornerTextureCoordinates.y);

			// emplace back bottom right corner
			const sf::Vector2f bottomRightPositions = tileBounds.getBottomRight();
			vertices.emplace_back(bottomRightPositions.x);
			vertices.emplace_back(bottomRightPositions.y);
			const sf::Vector2f bottomRightCornerTextureCoordinates = getTextureCoordinate(tileData, 2);
			vertices.emplace_back(bottomRightCornerTextureCoordinates.x);
			vertices.emplace_back(bottomRightCornerTextureCoordinates.y);

			// emplace back bottom left corner
			const sf::Vector2f bottomLeftPositions = tileBounds.getBottomLeft();
			vertices.emplace_back(bottomLeftPositions.x);
			vertices.emplace_back(bottomLeftPositions.y);
			const sf::Vector2f bottomLeftCornerTextureCoordinates = getTextureCoordinate(tileData, 3);
			vertices.emplace_back(bottomLeftCornerTextureCoordinates.x);
			vertices.emplace_back(bottomLeftCornerTextureCoordinates.y);
		}
	}

	VertexBuffer vbo = createVertexBuffer();
	setData(vbo, vertices.data(), vertices.size() * sizeof(float), DataUsage::staticDraw);
	mVertexArray->setVertexBuffer(vbo, VertexBufferLayout::position2_texCoords2);

	// create index buffer data 
	std::vector<unsigned> indices;
	constexpr unsigned numberOfIndicesPerTile = 6;
	indices.resize(chunkSizeInTiles.x * chunkSizeInTiles.y * numberOfIndicesPerTile);

	// TODO: Refactor this mess
	unsigned nrOfTile = 0;
	unsigned nrOfIndexInTile = 0;
	for(;;)
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

		if(nrOfIndexInChunk + 1 == indices.size())
			break;
	}

	IndexBuffer ibo = createIndexBuffer();
	setData(ibo, indices.data(), indices.size());
	mVertexArray->setIndexBuffer(ibo);


	mTilesToCreate.clear();
}

void Chunk::draw() const
{
	auto* shader = ShaderLibrary::getInstance().get("textureSprite");
	Renderer::submit(*mVertexArray, *shader, getGlobalBounds());
}

sf::FloatRect Chunk::getGlobalBounds() const
{
	const sf::Vector2f chunkSizeInPixels = static_cast<sf::Vector2f>(mChunkData->getChunkSizeInPixels());
	return sf::FloatRect(mTopLeftCornerPositionInWorld.x, mTopLeftCornerPositionInWorld.y, chunkSizeInPixels.x, chunkSizeInPixels.y);
}

sf::Vector2f Chunk::getTextureCoordinate(const TileData& tileData, unsigned vertexIndexInTile) const
{
	// TODO: Refactor and Optimize this mess

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
		textureIndices = {3, 0, 1, 2};
	else if(isVerticallyFlipped && isDiagonallyFlipped)
		textureIndices = {1, 2, 3, 0};
	else if(isHorizontallyFlipped)
		textureIndices = {1, 0, 3, 2};
	else if(isVerticallyFlipped)
		textureIndices = {3, 2, 1, 0};
	else if(isDiagonallyFlipped)
		textureIndices = {0, 3, 2, 1};

	const unsigned flippedIndexInTile = textureIndices[vertexIndexInTile];

	// tile texture rect positions
	const sf::Vector2f tileSizeInPixels = static_cast<sf::Vector2f>(mChunkData->getTileSizeInPixels());
	const sf::Vector2f textureRectTopLeftCorner = static_cast<sf::Vector2f>(tileData.mTextureRectTopLeftCorner);
	const sf::Vector2f textureSize = static_cast<sf::Vector2f>(mChunkData->getTileset().getSize());
	const FloatRect textureRect(
		textureRectTopLeftCorner.x / textureSize.x,
		(textureSize.y - textureRectTopLeftCorner.y) / textureSize.y,
		tileSizeInPixels.x / textureSize.x,
		tileSizeInPixels.y / textureSize.y
	);

	switch(flippedIndexInTile)
	{
	case 0:
		return textureRect.getTopLeft();
	case 1:
		return textureRect.getTopRight();
	case 2:
		return {textureRect.right(), textureRect.top - textureRect.height};
	case 3:
		return {textureRect.left, textureRect.top - textureRect.height};
	default:
		break;
	}
}

}
