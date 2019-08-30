#pragma once

#include "GameObjects/gameObject.hpp"

#include <deque>

namespace ph {

class Player;
class Item;
class ItemsContainer;

class Equipement : public GameObject
{
public:
	Equipement();
	void init();

	void updateCurrent(sf::Time delta) override;

	void dropAllItems();

private:
	void handleInteractableItems();
	void handlePickArea();

	void dropItem(Item* itemToDrop);
	void pickUpItem(Item* itemToPick);

	auto getItemsContainer()->ItemsContainer &;

private:
	Player* mInventoryOwner;
	ItemsContainer* mItemsContainer;
	std::deque<Item*> mEquipementStash;
	std::deque<Item*> mInteractableItems;
};

}