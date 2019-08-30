#include "GameObjects/NotDrawableGameObjects/equipement.hpp"
#include "GameObjects/GameObjectContainers/itemsContainer.hpp"
#include "GameObjects/DrawableGameObjects/item.hpp"
#include "Utilities/math.hpp"

namespace ph {

Equipement::Equipement()
	:GameObject("Equipement")
{
}

void Equipement::updateCurrent(sf::Time delta) 
{
	auto& inventoryOwner = getParent();
	auto& itemsOnTheGround = getItemsContainer().getChildren();

	//Will be divided into methods

	for (auto& item : itemsOnTheGround)
		if (Math::isPointInsideCircle(item->getPosition(), inventoryOwner.getPosition(), 20))
		{
			auto& interactableItem = dynamic_cast<Item&>((*item));
			interactableItem.setInteractable(true);
			mInteractableItems.emplace_back(&interactableItem);
		}

	for (auto it = mInteractableItems.begin(); it != mInteractableItems.end(); ++it)
	{
		pickUpItem(*it);
		(*it)->setInteractable(false);
	}

	mInteractableItems.clear();

	//if (mC.getElapsedTime().asSeconds() > 7)
	//{
	//	dropItem(mEquipementStash[0]);
	//	mC.restart();
	//}
	//Dropping was tested and works properly
}

void Equipement::pickUpItem(Item* itemToPick)
{
	itemToPick->onPickUp();
	itemToPick->setInInventory(true);
	getItemsContainer().changeParentOfChild(itemToPick, this);
	mEquipementStash.emplace_back(itemToPick);
}

void Equipement::dropItem(Item* itemToDrop)
{
	for (auto it = mEquipementStash.begin(); it != mEquipementStash.end(); ++it)
		if (*it == itemToDrop)
		{
			itemToDrop->onDrop();
			itemToDrop->setInInventory(false);
			changeParentOfChild(itemToDrop, &dynamic_cast<GameObject&>(getItemsContainer()));
			mEquipementStash.erase(it);
			return;
		}
}

auto Equipement::getItemsContainer() -> ItemsContainer &
{
	auto& player = getParent();
	auto& standingObjects = player.getParent();
	return dynamic_cast<ItemsContainer&>(standingObjects.getChild("ItemsContainer"));
}

}