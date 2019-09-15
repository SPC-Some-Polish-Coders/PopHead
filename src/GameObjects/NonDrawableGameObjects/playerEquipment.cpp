#include "GameObjects/NonDrawableGameObjects/playerEquipment.hpp"
#include "Utilities/math.hpp"
#include "GameObjects/GameObjectContainers/itemsContainer.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "GameObjects/DrawableGameObjects/item.hpp"

namespace ph {

PlayerEquipment::PlayerEquipment()
{
}

void PlayerEquipment::init()
{
	Equipment::init();
	mItemsCounter = { 
		{"Bullet", 0},
		{"Medkit", 0} 
	};
	mOwnerPickRadius = dynamic_cast<Player&>(*mInventoryOwner).getPickRadius();
	countItems();
}

void PlayerEquipment::countItems()
{
	for (auto& item : mEquipmentStash)
		mItemsCounter.at(getNameWithoutCounters(item->getName())) = 0;

	for (auto& item : mEquipmentStash)
		++mItemsCounter.at(getNameWithoutCounters(item->getName()));
}

int PlayerEquipment::getItemQuantity(const std::string& name)
{
	return mItemsCounter.at(name);
}

void PlayerEquipment::updateCurrent(sf::Time delta)
{
	handlePickArea();
	handleInteractableItems();
}

void PlayerEquipment::handlePickArea()
{
	auto& itemsOnTheGround = getItemsContainer().getChildren();
	for (auto& item : itemsOnTheGround)
		if (Math::isPointInsideCircle(item->getPosition(), mInventoryOwner->getPosition(), mOwnerPickRadius))
		{
			auto& interactableItem = dynamic_cast<Item&>((*item));
			interactableItem.setInteractable(true);
			mInteractableItems.emplace_back(&interactableItem);
		}
}

void PlayerEquipment::handleInteractableItems()
{
	for (auto it = mInteractableItems.begin(); it != mInteractableItems.end(); ++it)
	{
		pickUpItem(*it);
		(*it)->setInteractable(false);
	}

	mInteractableItems.clear();
}

void PlayerEquipment::pickUpItem(Item* itemToPick)
{
	incrementQuantityOfItem(itemToPick->getName());
	itemToPick->setInInventory(true);
	getItemsContainer().changeParentOfChild(itemToPick, this);
	mEquipmentStash.emplace_back(itemToPick);
	itemToPick->onPickUp();
}

void PlayerEquipment::putItem(std::unique_ptr<Item> itemToPut)
{
	incrementQuantityOfItem(itemToPut->getName());
	Equipment::putItem(std::move(itemToPut));
}

void PlayerEquipment::dropItem(Item* itemToDrop)
{
	decrementQuantityOfItem(itemToDrop->getName());
	Equipment::dropItem(itemToDrop);
}

void PlayerEquipment::destroyItem(Item* itemToDestroy)
{
	for (auto it = mEquipmentStash.begin(); it != mEquipmentStash.end(); ++it)
		if (*it == itemToDestroy)
		{
			removeChild(*it);
			mEquipmentStash.erase(it);
			decrementQuantityOfItem(itemToDestroy->getName());
			return;
		}
}

void PlayerEquipment::destroyItem(const std::string& itemToDestroy)
{
	for (auto it = mEquipmentStash.begin(); it != mEquipmentStash.end(); ++it)
		if (itemToDestroy.find_first_of((*it)->getName()) != std::string::npos)
		{
			removeChild(*it);
			mEquipmentStash.erase(it);
			decrementQuantityOfItem(itemToDestroy);
			return;
		}
}

void PlayerEquipment::incrementQuantityOfItem(const std::string& itemName)
{
	++mItemsCounter.at(getNameWithoutCounters(itemName));
}

void PlayerEquipment::decrementQuantityOfItem(const std::string& itemName)
{
	--mItemsCounter.at(getNameWithoutCounters(itemName));
}

std::string PlayerEquipment::getNameWithoutCounters(const std::string& name)
{
	if (name.find('_') == std::string::npos)
		return name;

	std::string itemName = name;
	std::size_t numberPosition = itemName.find_first_of('_');
	itemName.erase(numberPosition);
	return itemName;
}

}