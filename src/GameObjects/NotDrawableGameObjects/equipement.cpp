#include "GameObjects/NotDrawableGameObjects/equipement.hpp"
#include "GameObjects/GameObjectContainers/itemsContainer.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "GameObjects/DrawableGameObjects/item.hpp"
#include "GameObjects/gameObject.hpp"
#include "Utilities/math.hpp"

namespace ph {

Equipement::Equipement()
	:GameObject("Equipement")
	,mItemsContainer(nullptr)
	,mInventoryOwner(nullptr)
{
}

void Equipement::init()
{
	mInventoryOwner = &dynamic_cast<Player&>(getParent());
	mItemsContainer = &getItemsContainer();
}

void Equipement::updateCurrent(sf::Time delta) 
{
	handlePickArea();
	handleInteractableItems();
}

void Equipement::handlePickArea()
{
	auto& itemsOnTheGround = getItemsContainer().getChildren();
	for (auto& item : itemsOnTheGround)
		if (Math::isPointInsideCircle(item->getPosition(), mInventoryOwner->getPosition(), mInventoryOwner->getPickRadius()))
		{
			auto& interactableItem = dynamic_cast<Item&>((*item));
			interactableItem.setInteractable(true);
			mInteractableItems.emplace_back(&interactableItem);
		}
}

void Equipement::handleInteractableItems()
{
	for (auto it = mInteractableItems.begin(); it != mInteractableItems.end(); ++it)
	{
		pickUpItem(*it);
		(*it)->setInteractable(false);
	}

	mInteractableItems.clear();
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
			itemToDrop->setPosition(mInventoryOwner->getPosition());
			changeParentOfChild(itemToDrop, &dynamic_cast<GameObject&>(getItemsContainer()));
			mEquipementStash.erase(it);
			return;
		}
}

auto Equipement::getItemsContainer() -> ItemsContainer &
{
	auto& standingObjects = mRoot->getChild("LAYER_standingObjects");
	return dynamic_cast<ItemsContainer&>(standingObjects.getChild("ItemsContainer"));
}

}