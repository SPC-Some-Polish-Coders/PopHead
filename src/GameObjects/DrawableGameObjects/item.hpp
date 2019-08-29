#pragma once

#include "GameObjects/drawableGameObject.hpp"

namespace ph {

class GameData;

class Item : public DrawableGameObject
{
public:
	Item(GameData* const gameData, const std::string& name);

	virtual void onUse();
	virtual void onPickUp();
	virtual void onDrop();
	//Some other virtuals

	void setClickable(const bool clickable) { mClickable = clickable; }
	bool getClickable() { return mClickable; }

private:
	bool mClickable;
	bool mInInventory;
};

}