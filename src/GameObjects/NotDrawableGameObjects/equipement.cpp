#include "GameObjects/NotDrawableGameObjects/equipement.hpp"
#include "GameObjects/DrawableGameObjects/item.hpp"
#include "Renderer/renderer.hpp"
#include "Utilities/math.hpp"
#include "GameObjects/DrawableGameObjects/item.hpp"
#include "GameObjects/rect.hpp"

namespace ph {

Equipement::Equipement()
	:GameObject("Equipement")
{
}

void Equipement::updateCurrent(sf::Time delta) 
{
	//auto& player = getParent();
	//auto& items = getParent().getParent().getChild("ItemsContainer").getChildren();

	//if(!items.empty())
	//for (auto& item : items)
	//{
	//	if (Math::isPointInsideRect(item->getPosition(), player.getGlobalBounds()))
	//	{
	//		pickUpItem(&dynamic_cast<Item&>(*item));
	//		return;
	//	}
	//}
	//get Equipement's parent
	//check if parent performed any actions to drop an item, if yes
		//dropItem(item)

	//check if any of the items in the area are in the parent's pick-radius,
	//if yes, make them clickable

	//check if parent performed any actions to pick up an item, if yes
		//pickUpItem(item)
}

void Equipement::pickUpItem(Item* itemToPick)
{
	//for (auto& item : mEquipementStash)
	//	if (itemToPick == item)
	//		return;

	//itemToPick->onPickUp();
	//itemToPick->setInInventory(true);
	//getParent().getParent().getChild("ItemsContainer").changeParentOfChild(itemToPick, this);
	////get equipement's parent and add an item as its child
	//mEquipementStash.emplace_back(itemToPick);
}

void Equipement::dropItem(Item* itemToDrop)
{
	//for (auto it = mEquipementStash.begin(); it != mEquipementStash.end(); ++it)
	//{
	//	if (*it == itemToDrop)
	//	{
	//		itemToDrop->onDrop();
	//		mEquipementStash.erase(it);
	//	}
	//}
}

}