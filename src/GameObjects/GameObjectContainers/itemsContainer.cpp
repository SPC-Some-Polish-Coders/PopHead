#include "GameObjects/GameObjectContainers/itemsContainer.hpp"
#include "GameObjects/DrawableGameObjects/item.hpp"

namespace ph {

ItemsContainer::ItemsContainer()
	:GameObject("ItemsContainer")
{
}

void ItemsContainer::drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const
{

}

void ItemsContainer::updateCurrent(const sf::Time time) 
{

}

}