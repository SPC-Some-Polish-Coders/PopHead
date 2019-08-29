#include "GameObjects/DrawableGameObjects/item.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

Item::Item(GameData* const gameData, const std::string& name)
	:DrawableGameObject(gameData->getRenderer(), name, LayerID::floorEntities)
{
}

void Item::onUse()
{
	PH_LOG_INFO("This item has no usage");
}

}