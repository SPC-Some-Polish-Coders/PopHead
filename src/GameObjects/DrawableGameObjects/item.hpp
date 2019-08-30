#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class Item : public GameObject
{
public:
	Item(const std::string& name);

	virtual void onUse();
	virtual void onPickUp();
	virtual void onDrop();

	virtual void drawWhileOnTheGround(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	virtual void updateWhileOnTheGround(const sf::Time time) = 0;

	void updateCurrent(const sf::Time);
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setInteractable(const bool interactable) { mInteractable = interactable; }
	bool getInteractable() const { return mInteractable; }
	void setInInventory(const bool inInv) { mInInventory = inInv; }
	bool getInInventory() const { return mInInventory; }

private:
	bool mInteractable;
	bool mInInventory;
};

}