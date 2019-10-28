#include "GameObjects/DrawableGameObjects/Items/bulletItem.hpp"

namespace ph {

BulletItem::BulletItem(GameData* const gameData)
	:Item(gameData, "Bullet", gameData->getTextures().get("textures/others/bulletGround.png"))
{
}

}
