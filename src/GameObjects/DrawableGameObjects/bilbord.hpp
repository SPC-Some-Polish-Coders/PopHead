#pragma once 

#include "GameObjects/gameObject.hpp"

namespace ph { 
 
class Bilbord : public GameObject
{
public:
	Bilbord(const sf::Texture& stayingBilbordTexture, const sf::Texture& lyingBilbordTexture);

	void drawCurrent(sf::RenderTarget&, const sf::RenderStates) const override;

	void fallOver();

private:
	sf::Sprite mSprite;
	const sf::Texture& mStayingBilbordTexture;
	const sf::Texture& mLyingBilbordTexture;
};

} 
