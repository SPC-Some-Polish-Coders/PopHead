#include "GameObjects/DrawableGameObjects/Items/medkit.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"

namespace ph {

Medkit::Medkit(GameData* const gameData)
	:Item(gameData, "Medkit")
{
	setGroundTexture(gameData->getTextures().get("textures/others/medkit.png"));
}

void Medkit::drawWhileOnTheGround(sf::RenderTarget& rt, sf::RenderStates rs) const
{
	rt.draw(getGroundSprite());
}

void Medkit::onPickUp()
{
	auto* playerGameObject = mRoot->getChild("LAYER_standingObjects")->getChild("player");
	if (playerGameObject == nullptr)
		return;
	dynamic_cast<Player*>(playerGameObject)->addHp(25);
}

}