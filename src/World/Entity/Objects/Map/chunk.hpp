#pragma once

#include "SFML/Graphics.hpp"
#include <array>

namespace ph {

struct Tile
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
	Chunk(const sf::Texture& tileset);

	void addTile(const Tile& tile) { mTilesToCreate.emplace_back(tile); };

	void create();

	void draw(sf::RenderTarget&, sf::RenderStates) const override;

private:
	sf::VertexArray mVertexArray;
	std::vector<Tile> mTilesToCreate;
	const sf::Texture& mTileset;
};

}