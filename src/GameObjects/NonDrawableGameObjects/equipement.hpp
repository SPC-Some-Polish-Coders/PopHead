#pragma once

#include "GameObjects/gameObject.hpp"

#include <deque>

namespace ph {

class Character;
class Item;
class ItemsContainer;

class Equipement : public GameObject
{
public:
	Equipement();
	void init();
	void putItem(std::unique_ptr<Item> itemToPut);

	void dropAllItems();

	void dropItem(Item* itemToDrop);

	auto getItemsContainer()->ItemsContainer &;

protected:
	Character* mInventoryOwner;
	ItemsContainer* mItemsContainer;
	std::deque<Item*> mEquipementStash;
};

}