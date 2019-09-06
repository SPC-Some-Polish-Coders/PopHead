#pragma once

#include "GameObjects/NonDrawableGameObjects/equipement.hpp"

namespace ph {

class PlayerEquipement : public Equipement
{
public:
	PlayerEquipement();

	void updateCurrent(sf::Time delta) override;

private:
	void handleInteractableItems();
	void handlePickArea();

	void pickUpItem(Item* itemToPick);

private:
	std::deque<Item*> mInteractableItems;
};

}
