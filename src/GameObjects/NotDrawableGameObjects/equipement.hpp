#pragma once

#include "GameObjects/gameObject.hpp"

#include <deque>

namespace ph {

class Item;
class ItemsContainer;

class Equipement : public GameObject
{
public:
	Equipement();

	void updateCurrent(sf::Time delta) override;

	void pickUpItem(Item* itemToPick);
	void dropItem(Item* itemToDrop);

	//void openEqStash();
	//void closeEqStash();

private:
	auto getItemsContainer()->ItemsContainer &;

private:
	std::deque<Item*> mEquipementStash;
	std::vector<Item*> mInteractableItems;
};

}