#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class GameData;

class Item : public GameObject
{
public:
	Item(GameData* const gameData, const std::string& name);

	virtual void onUse();
	virtual void onPickUp();
	virtual void onDrop();
	//Some other virtuals

	void setClickable(const bool clickable) { mClickable = clickable; }
	bool getClickable() { return mClickable; }
	void setInInventory(const bool inInv) { mInInventory = inInv; }
	bool getInInventory(){ return mInInventory; }

private:
	bool mClickable;
	bool mInInventory;
};

}