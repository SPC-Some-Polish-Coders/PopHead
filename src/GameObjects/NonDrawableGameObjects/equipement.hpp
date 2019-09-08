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

	virtual void init();
	virtual void dropItem(Item* itemToDrop);
	virtual void putItem(std::unique_ptr<Item> itemToPut);

	void dropAllItems();

	auto getItemsContainer()->ItemsContainer &;

protected:
	Character* mInventoryOwner;
	ItemsContainer* mItemsContainer;
	std::deque<Item*> mEquipementStash;
};

}