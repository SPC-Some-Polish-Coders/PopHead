#pragma once

#include "GameObjects/drawableGameObject.hpp"

namespace ph {

class GameData;

class Item : public DrawableGameObject
{
public:
	Item(GameData* const gameData, const std::string& name);

	virtual void onUse();
	//Some other virtuals
};

}