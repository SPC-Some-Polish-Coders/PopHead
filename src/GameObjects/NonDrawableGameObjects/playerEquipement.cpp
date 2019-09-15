#include "GameObjects/NonDrawableGameObjects/playerEquipement.hpp"
#include "Utilities/math.hpp"
#include "GameObjects/GameObjectContainers/itemsContainer.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "GameObjects/DrawableGameObjects/item.hpp"

namespace ph {

PlayerEquipement::PlayerEquipement()
{
}

void PlayerEquipement::init()
{
	Equipement::init();
	mItemsCounter = { 
		{"Bullet", 0},
		{"Medkit", 0} 
	};
	mOwnerPickRadius = dynamic_cast<Player&>(*mInventoryOwner).getPickRadius();
	countItems();
}

void PlayerEquipement::countItems()
{
	for (auto& item : mEquipementStash)
		mItemsCounter.at(getNameWithoutCounters(item->getName())) = 0;

	for (auto& item : mEquipementStash)
		++mItemsCounter.at(getNameWithoutCounters(item->getName()));
}

int PlayerEquipement::getItemQuantity(const std::string& name)
{
	return mItemsCounter.at(name);
}

void PlayerEquipement::updateCurrent(sf::Time delta)
{
	handlePickArea();
	handleInteractableItems();
}

void PlayerEquipement::handlePickArea()
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
	incrementQuantityOfItem(itemToPick->getName());
	itemToPick->onPickUp();
	itemToPick->setInInventory(true);
	getItemsContainer().changeParentOfChild(itemToPick, this);
	mEquipementStash.emplace_back(itemToPick);
}

void PlayerEquipement::putItem(std::unique_ptr<Item> itemToPut)
{
	incrementQuantityOfItem(itemToPut->getName());
	Equipement::putItem(std::move(itemToPut));
}

void PlayerEquipement::dropItem(Item* itemToDrop)
{
	decrementQuantityOfItem(itemToDrop->getName());
	Equipement::dropItem(itemToDrop);
}

void PlayerEquipement::destroyItem(Item* itemToDestroy)
{
	for (auto it = mEquipementStash.begin(); it != mEquipementStash.end(); ++it)
		if (*it == itemToDestroy)
		{
			removeChild(*it);
			mEquipementStash.erase(it);
			decrementQuantityOfItem(itemToDestroy->getName());
			return;
		}
}

void PlayerEquipement::destroyItem(const std::string& itemToDestroy)
{
	for (auto it = mEquipementStash.begin(); it != mEquipementStash.end(); ++it)
		if (itemToDestroy.find_first_of((*it)->getName()) != std::string::npos)
		{
			removeChild(*it);
			mEquipementStash.erase(it);
			decrementQuantityOfItem(itemToDestroy);
			return;
		}
}

void PlayerEquipement::incrementQuantityOfItem(const std::string& itemName)
{
	++mItemsCounter.at(getNameWithoutCounters(itemName));
}

void PlayerEquipement::decrementQuantityOfItem(const std::string& itemName)
{
	--mItemsCounter.at(getNameWithoutCounters(itemName));
}

std::string PlayerEquipement::getNameWithoutCounters(const std::string& name)
{
	if (name.find('_') == std::string::npos)
		return name;

	std::string itemName = name;
	std::size_t numberPosition = itemName.find_first_of('_');
	itemName.erase(numberPosition);
	return itemName;
}

}