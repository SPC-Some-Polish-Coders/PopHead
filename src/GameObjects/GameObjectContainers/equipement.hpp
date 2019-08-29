#pragma once

#include "GameObjects/gameObject.hpp"

#include <Vector>

namespace ph {

class Item;
class Renderer;

class Equipement : public GameObject
{
public:
	Equipement();

	void updateCurrent(sf::Time delta) override;

	void pickUpItem(Item* const itemToPick);
	void dropItem(Item* const itemToDrop);

	//void openEqStash();
	//void closeEqStash();

	//some more methods

private:
	std::vector<Item*> mEquipementStash;
};

}