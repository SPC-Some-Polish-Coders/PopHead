#pragma once

#include "GameObjects/gameObject.hpp"

#include <deque>

namespace ph {

class Item;

class ItemsContainer : public GameObject
{
public:
	ItemsContainer();

	void updateCurrent(const sf::Time time) override;
	void drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const override;

};

}