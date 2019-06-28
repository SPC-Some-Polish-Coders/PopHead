#pragma once

#include "SFML/Graphics.hpp"
#include <array>

namespace ph {

struct FlippingData
{
	bool mIsHorizontallyFlipped;
	bool mIsVerticallyFlipped;
	bool mIsDiagonallyFlipped;
};

struct TileData
{
	sf::Vector2f mTopLeftCornerPositionInWorld;
	sf::Vector2u mTextureRectTopLeftCorner;
	FlippingData mFlippingData;
};

class Chunk : public sf::Drawable
{
public:
	explicit Chunk(const sf::Texture& tileset, const sf::Vector2f topLeftCornerPositionInWorld);

	void addTileData(const TileData& tile) { mTilesToCreate.emplace_back(tile); };

	void initializeGraphics();

	void draw(sf::RenderTarget&, sf::RenderStates) const override;

	sf::FloatRect getGlobalBounds() const;

private:
	sf::VertexArray mVertexArray;
	std::vector<TileData> mTilesToCreate;
	const sf::Texture& mTileset;
	const sf::Vector2f mTopLeftCornerPositionInWorld;
};

}