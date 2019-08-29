#include "GameObjects/GameObjectContainers/equipement.hpp"
#include "GameObjects/DrawableGameObjects/item.hpp"
#include "Renderer/renderer.hpp"

namespace ph {

Equipement::Equipement(Renderer& renderer)
	:GameObject("Equipement")
	,mRenderer(renderer)
{
}

void Equipement::updateCurrent(sf::Time delta) 
{
	//get Equipement's parent
	//check if parent performed any actions to drop an item, if yes
		//dropItem(item)

	//check if any of the items in the area are in the parent's pick-radius,
	//if yes, make them clickable

	//check if parent performed any actions to pick up an item, if yes
		//pickUpItem(item)
}

void Equipement::pickUpItem(Item* const itemToPick)
{
	for (auto& item : mEquipementStash)
		if (itemToPick == item)
			return;

	itemToPick->onPickUp();
	//get equipement's parent and add an item as its child
	mRenderer.removeDrawableGameObject(itemToPick);
	mEquipementStash.emplace_back(itemToPick);
}

void Equipement::dropItem(Item* const itemToDrop)
{
	for (auto it = mEquipementStash.begin(); it != mEquipementStash.end(); ++it)
	{
		if (*it == itemToDrop)
		{
			itemToDrop->onDrop();
			mEquipementStash.erase(it);
			mRenderer.addObject(itemToDrop);
		}
	}
}

}