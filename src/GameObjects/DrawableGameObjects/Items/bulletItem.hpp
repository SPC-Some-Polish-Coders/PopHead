#pragma once

#include "GameObjects/DrawableGameObjects/item.hpp"

namespace ph {

class GameData;

class BulletItem : public Item
{
public:
	BulletItem(GameData* const gameData);

	void onUse() override {}
	void onPickUp() override {}
	void onDrop() override {}

	void drawWhileOnTheGround(sf::Transform) override;

	void updateWhileOnTheGround(const sf::Time time) override {}
};

}