#pragma once

#include "GameObjects/DrawableGameObjects/item.hpp"

namespace ph {

class GameData;

class Medkit : public Item
{
public:
	Medkit(GameData* const gameData);

	void drawWhileOnTheGround(sf::RenderTarget& rt, sf::RenderStates rs) const;
	void updateWhileOnTheGround(const sf::Time delta) override {}
	void onPickUp();

private:
	int healthAddValue;
};

}