#include "GameObjects/NonDrawableGameObjects/equipement.hpp"
#include "GameObjects/GameObjectContainers/itemsContainer.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "GameObjects/DrawableGameObjects/item.hpp"
#include "GameObjects/gameObject.hpp"
#include "Utilities/math.hpp"
#include "Utilities/random.hpp"

namespace ph {

Equipement::Equipement()
	:GameObject("Equipement")
	,mItemsContainer(nullptr)
	,mInventoryOwner(nullptr)
{
}

void Equipement::init()
{
	mInventoryOwner = &dynamic_cast<Character&>(getParent());
	mItemsContainer = &getItemsContainer();
}

void Equipement::putItem(std::unique_ptr<Item> itemToPut)
{
	itemToPut->setInInventory(true);
	mEquipementStash.emplace_back(itemToPut.get());
	addChild(std::move(itemToPut));
}

void Equipement::dropAllItems()
{
	for (auto& item : mEquipementStash)
		dropItem(item);
}

void Equipement::dropItem(Item* itemToDrop)
{
	for (auto it = mEquipementStash.begin(); it != mEquipementStash.end(); ++it)
		if (*it == itemToDrop)
		{
			itemToDrop->onDrop();
			itemToDrop->setInInventory(false);
			itemToDrop->setPosition(mInventoryOwner->getWorldPosition()+sf::Vector2f(Random::generateNumber(-10.f, 10.f), Random::generateNumber(-10.f, 10.f)));
			changeParentOfChild(itemToDrop, &dynamic_cast<GameObject&>(getItemsContainer()));
			mEquipementStash.erase(it);
			return;
		}
}

auto Equipement::getItemsContainer() -> ItemsContainer &
{
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");
	return dynamic_cast<ItemsContainer&>(*standingObjects->getChild("ItemsContainer"));
}

}