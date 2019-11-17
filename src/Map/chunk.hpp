#pragma once

#include "chunkData.hpp"
#include "Renderer/Vertices/vertexArray.hpp"
#include <SFML/System/Vector2.hpp>
#include <Utilities/rect.hpp>
#include <array>
#include <vector>

namespace ph {

struct FlipData
{
	bool mIsHorizontallyFlipped;
	bool mIsVerticallyFlipped;
	bool mIsDiagonallyFlipped;
};

struct TileData
{
	sf::Vector2f mTopLeftCornerPositionInWorld;
	sf::Vector2u mTextureRectTopLeftCorner;
	FlipData mFlipData;
};

class Chunk
{
public:
	explicit Chunk(const sf::Vector2f topLeftCornerPositionInWorld, std::shared_ptr<ChunkData>);

	void addTileData(const TileData& tile);

	void initializeGraphics();

	void draw() const;

	sf::FloatRect getGlobalBounds() const;

private:
	sf::Vector2f getTextureCoordinate(const TileData& tileData, unsigned vertexIndexInTile) const;

private:
	std::vector<TileData> mTilesToCreate;
	const sf::Vector2f mTopLeftCornerPositionInWorld;
	std::shared_ptr<ChunkData> mChunkData;
	std::shared_ptr<VertexArray> mVertexArray;
};

}
