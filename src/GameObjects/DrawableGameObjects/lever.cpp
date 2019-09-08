#include "lever.hpp"
#include "character.hpp"
#include "Utilities/math.hpp"
#include "gate.hpp"

namespace ph {

Lever::Lever(sf::Texture& texture)
	:GameObject("lever")
	,mSprite(texture, {0, 0, 8, 16})
	,mIsLeverDown(false)
{
}

void Lever::updateCurrent(const sf::Time delta)
{
	if(mIsLeverDown)
		return;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		auto* playerGameObject = mRoot->getChild("LAYER_standingObjects")->getChild("player");
		if(playerGameObject == nullptr)
			return;
		auto* player = dynamic_cast<Character*>(playerGameObject);
		if(Math::areTheyOverlapping(sf::FloatRect(getPosition().x, getPosition().y, 16, 8), player->getGlobalBounds())) {
			mIsLeverDown = true;
			mSprite.setTextureRect({8, 0, 8, 16});
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
	target.draw(mSprite, states);
}

}
