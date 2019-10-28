#include "GameObjects/DrawableGameObjects/item.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

Item::Item(GameData* const gameData, const std::string& name, const Texture& groundTexture)
	:GameObject(name)
	,mTexture(groundTexture)
	,mGameData(gameData)
	,mInteractable(false)
	,mInInventory(false)
{
}

void Item::updateCurrent(const sf::Time time)
{
	if (!getInInventory())
		updateWhileOnTheGround(time);
}

void Item::drawCurrent(sf::Transform transform)
{
	if(!getInInventory())
		Renderer::submitQuad(mTexture, getPosition(), mTexture.getSize());
}

void Item::setPosition(const sf::Vector2f& position)
{
	sf::Transformable::setPosition(position);
	//mGroundSprite.setPosition(position);
}

void Item::onUse()
{
	PH_LOG_INFO("This item has no usage");
}

void Item::onPickUp()
{
	PH_LOG_INFO("This item has no pick up reaction");
}

void Item::onDrop()
{
	PH_LOG_INFO("This item has no drop reaction");
}

}
