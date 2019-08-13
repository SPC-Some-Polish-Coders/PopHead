#pragma once

#include "GameObjects/DrawableGameObjects/Characters/npc.hpp"

namespace ph {

class GameData;

class CrawlingNpc : public Npc
{
public:
	CrawlingNpc(GameData* const);

	void update(const sf::Time delta);
	void die();

private:
	bool mIsDead;
};

}
