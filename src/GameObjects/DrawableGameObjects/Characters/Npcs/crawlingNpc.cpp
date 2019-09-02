#include "crawlingNpc.hpp"
#include "gameData.hpp"

namespace ph {

CrawlingNpc::CrawlingNpc(GameData* const gameData)
	:Npc(gameData, "crawlingNpc")
	,mIsDead(false)
{
	setPosition({5673, 396});
	mSprite.setTexture(gameData->getTextures().get("textures/characters/negroDudeWalkingAnimation.png"));
	mAnimation.changeState("crawlingUp");
}

void CrawlingNpc::updateCurrent(const sf::Time delta)
{
	if(!mIsDead) {
		move(sf::Vector2f(0, -1) * delta.asSeconds());
		mAnimation.animate(mSprite, delta);
	}
}

void CrawlingNpc::die()
{
	mIsDead = true;
	mAnimation.changeState("dead");
	mAnimation.animate(mSprite);
}

}