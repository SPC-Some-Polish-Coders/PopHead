#include "lever.hpp"
#include "character.hpp"
#include "Utilities/rect.hpp"
#include "gate.hpp"

namespace ph {

Lever::Lever(sf::Texture& leverTexture, sf::Texture& hintTexture)
	:GameObject("lever")
	,mLeverSprite(leverTexture, {0, 0, 8, 16})
	,mHintSprite(hintTexture)
	,mIsLeverDown(false)
	,mIsPlayerInHintArea(false)
{
	mHintSprite.setPosition(-70.f, 15.f);
}

void Lever::updateCurrent(const sf::Time delta)
{
	if(mIsLeverDown)
		return;

	mIsPlayerInHintArea = false;

	auto* playerGameObject = mRoot->getChild("LAYER_standingObjects")->getChild("player");
	if(playerGameObject == nullptr)
		return;
	auto* player = dynamic_cast<Character*>(playerGameObject);

	const FloatRect hintArea(getPosition().x, getPosition().y, 12.f, 8.f);
	if(hintArea.doPositiveRectsIntersect(player->getGlobalBounds()))
	{
		mIsPlayerInHintArea = true;

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			mIsLeverDown = true;
			mIsPlayerInHintArea = false;
			mLeverSprite.setTextureRect({8, 0, 8, 16});
			auto* lyingObjects = mRoot->getChild("LAYER_lyingObjects");
			for(auto& child : lyingObjects->getChildren()) {
				auto* gate = dynamic_cast<Gate*>(child.get());
				if(gate == nullptr)
					continue;
				gate->open();
				return;
			}
		}
	}
}

void Lever::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mLeverSprite, states);
	if(mIsPlayerInHintArea)
		target.draw(mHintSprite, states);
}

}
