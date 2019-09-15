#include "GameObjects/NonDrawableGameObjects/equipment.hpp"
#include "GameObjects/GameObjectContainers/itemsContainer.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "GameObjects/DrawableGameObjects/item.hpp"
#include "GameObjects/gameObject.hpp"
#include "Utilities/math.hpp"
#include "Utilities/random.hpp"

namespace ph {

Equipment::Equipment()
	:GameObject("Equipment")
	,mItemsContainer(nullptr)
	,mInventoryOwner(nullptr)
{
}

void Equipment::init()
{
	mInventoryOwner = &dynamic_cast<Character&>(getParent());
	mItemsContainer = &getItemsContainer();
}

void Equipment::putItem(std::unique_ptr<Item> itemToPut)
{
	itemToPut->setInInventory(true);
	mEquipmentStash.emplace_back(itemToPut.get());
	addChild(std::move(itemToPut));
}

void Equipment::dropAllItems()
{
	for (auto& item : mEquipmentStash)
		dropItem(item);
}

void Equipment::dropItem(Item* itemToDrop)
{
	for (auto it = mEquipmentStash.begin(); it != mEquipmentStash.end(); ++it)
		if (*it == itemToDrop)
		{
			itemToDrop->onDrop();
			itemToDrop->setInInventory(false);
			itemToDrop->setPosition(mInventoryOwner->getWorldPosition()+sf::Vector2f(Random::generateNumber(-10.f, 10.f), Random::generateNumber(-10.f, 10.f)));
			changeParentOfChild(itemToDrop, &dynamic_cast<GameObject&>(getItemsContainer()));
			mEquipmentStash.erase(it);
			return;
		}
}

auto Equipment::getItemsContainer() -> ItemsContainer &
{
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");
	return dynamic_cast<ItemsContainer&>(*standingObjects->getChild("ItemsContainer"));
}

}