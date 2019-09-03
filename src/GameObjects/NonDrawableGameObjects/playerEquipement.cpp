#include "GameObjects/NonDrawableGameObjects/playerEquipement.hpp"
#include "Utilities/math.hpp"
#include "GameObjects/GameObjectContainers/itemsContainer.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "GameObjects/DrawableGameObjects/item.hpp"

namespace ph {

PlayerEquipement::PlayerEquipement()
{
}


void PlayerEquipement::updateCurrent(sf::Time delta)
{
	handlePickArea();
	handleInteractableItems();
}

void PlayerEquipement::handlePickArea()
{
	auto& itemsOnTheGround = getItemsContainer().getChildren();
	auto& elo = dynamic_cast<Player&>(*mInventoryOwner);
	for (auto& item : itemsOnTheGround)
		if (Math::isPointInsideCircle(item->getPosition(), mInventoryOwner->getPosition(), dynamic_cast<Player&>(*mInventoryOwner).getPickRadius()))
		{
			auto& interactableItem = dynamic_cast<Item&>((*item));
			interactableItem.setInteractable(true);
			mInteractableItems.emplace_back(&interactableItem);
		}
}

void PlayerEquipement::handleInteractableItems()
{
	for (auto it = mInteractableItems.begin(); it != mInteractableItems.end(); ++it)
	{
		pickUpItem(*it);
		(*it)->setInteractable(false);
	}

	mInteractableItems.clear();
}

void PlayerEquipement::pickUpItem(Item* itemToPick)
{
	itemToPick->onPickUp();
	itemToPick->setInInventory(true);
	getItemsContainer().changeParentOfChild(itemToPick, this);
	mEquipementStash.emplace_back(itemToPick);
}

}