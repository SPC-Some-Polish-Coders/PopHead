#include "lever.hpp"
#include "character.hpp"
#include "Utilities/rect.hpp"
#include "gate.hpp"

namespace ph {

Lever::Lever(const Texture& leverTexture, const Texture& hintTexture)
	:GameObject("lever")
	,mCurrentTextureRect(0, 0, 8, 16)
	,mLeverTexture(leverTexture)
	,mHintTexture(hintTexture)
	,mIsLeverDown(false)
	,mIsPlayerInHintArea(false)
{
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
			mCurrentTextureRect = {8, 0, 8, 16};
			auto* lyingObjects = mRoot->getChild("LAYER_lyingObjects");
			for(auto& child : lyingObjects->getChildren()) {
				auto* gate = dynamic_cast<Gate*>(child.get());
				if(gate == nullptr)
					continue;
				gate->open();
			}
		}
	}
}

void Lever::drawCurrent(sf::Transform transform)
{
	Renderer::submitQuad(mLeverTexture, mCurrentTextureRect, getPosition(), {8, 16});
	if(mIsPlayerInHintArea) {
		transform.translate(-70.f, 15.f);
		Renderer::submitQuad(mHintTexture, getPosition(), mHintTexture.getSize());
	}
}

}
