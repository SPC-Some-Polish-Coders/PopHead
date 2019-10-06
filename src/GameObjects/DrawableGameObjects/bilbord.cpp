#include "bilbord.hpp"
#include "Logs/logs.hpp"

namespace ph { 
 
Bilbord::Bilbord(const sf::Texture& stayingBilbordTexture, const sf::Texture& lyingBilbordTexture, const bool isLying)
	:GameObject("bilbord")
	,mSprite(isLying ? lyingBilbordTexture : stayingBilbordTexture)
	,mStayingBilbordTexture(stayingBilbordTexture)
	,mLyingBilbordTexture(lyingBilbordTexture)
{
}

void Bilbord::drawCurrent(sf::Transform)
{
	//renderTarget.draw(mSprite, states);
}

void Bilbord::fallOver()
{
	move(0.f, 40.f);
	mSprite.setTexture(mLyingBilbordTexture);
}

}  
