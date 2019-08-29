#include "GameObjects/DrawableGameObjects/item.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

Item::Item(GameData* const gameData, const std::string& name)
	:DrawableGameObject(gameData->getRenderer(), name, LayerID::floorEntities)
	,mClickable(false)
	,mInInventory(false)
{
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