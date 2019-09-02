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
	void onDrop() override;

	void drawWhileOnTheGround(sf::RenderTarget& target, sf::RenderStates states) const override;

	void updateWhileOnTheGround(const sf::Time time) override {}
};

}