#include "lootSpawner.hpp"
#include <memory>
#include "GameObjects/DrawableGameObjects/Items/medkit.hpp"
#include "GameObjects/DrawableGameObjects/Items/bulletItem.hpp"

namespace ph {

LootSpawner::LootSpawner(LootType type, GameObject* itemsContainer, GameData* gameData)
	:GameObject("lootSpawner")
	,mGameData(gameData)
	,mItemsContainer(itemsContainer)
	,mType(type)
{
}

void LootSpawner::spawnLoot()
{
	switch(mType)
	{
	case LootType::Medkit:
		{
			auto medkit = std::make_unique<Medkit>(mGameData);
			medkit->setPosition(getPosition());
			mItemsContainer->addChild(std::move(medkit));
		} break;
	case LootType::Bullets:
		{
			for(int i = 0; i < 50; ++i) {
				auto bulletPack = std::make_unique<BulletItem>(mGameData);
				bulletPack->setPosition(getPosition());
				mItemsContainer->addChild(std::move(bulletPack));
			}
		} break;
	default:
		break;
	}
}


}
