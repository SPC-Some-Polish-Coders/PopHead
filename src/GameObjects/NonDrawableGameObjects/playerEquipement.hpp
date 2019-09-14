#pragma once

#include "GameObjects/NonDrawableGameObjects/equipement.hpp"

namespace ph {

class Item;

class PlayerEquipement : public Equipement
{
public:
	PlayerEquipement();
	
	void updateCurrent(sf::Time delta) override;
	void init() override;

	void putItem(std::unique_ptr<Item> itemToPut) override;
	void dropItem(Item* itemToDrop) override;
	void destroyItem(Item* itemToDrop);
	void destroyItem(const std::string& itemToDrop);

	int getItemQuantity(const std::string& name);

private:
	void countItems();
	void handleInteractableItems();
	void handlePickArea();
	void pickUpItem(Item* itemToPick);

	void incrementQuantityOfItem(const std::string& itemName);
	void decrementQuantityOfItem(const std::string& itemName);
	std::string getNameWithoutCounters(const std::string& name);

private:
	std::deque<Item*> mInteractableItems;
	std::map<std::string, unsigned int> mItemsCounter;
	float mOwnerPickRadius;
};

}
