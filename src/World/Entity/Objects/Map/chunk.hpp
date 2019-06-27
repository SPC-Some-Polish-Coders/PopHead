#pragma once

#include "SFML/Graphics.hpp"
#include <array>

namespace ph {

struct TileData
{
	sf::Vector2f mTopLeftCornerPositionInWorld;
	sf::Vector2u mTextureRectTopLeftCorner;
	//TODO: Make rotaion and flipping possible
	/*bool mIsHorizontallyFlipped;
	bool mIsVerticallyFlipped;
	bool mIsDiagonallyFlipped;*/
};

class Chunk : public sf::Drawable
{
public:
	explicit Chunk(const sf::Texture& tileset);

	void addTile(const TileData& tile) { mTilesToCreate.emplace_back(tile); };

	void initializeGraphics();

	void draw(sf::RenderTarget&, sf::RenderStates) const override;

	sf::FloatRect getGlobalBounds() const;

private:
	sf::VertexArray mVertexArray;
	std::vector<TileData> mTilesToCreate;
	const sf::Texture& mTileset;
};

}