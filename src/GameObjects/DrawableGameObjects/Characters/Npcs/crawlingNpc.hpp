#pragma once

#include "GameObjects/DrawableGameObjects/Characters/npc.hpp"

namespace ph {

class GameData;

class CrawlingNpc : public Npc
{
public:
	CrawlingNpc(GameData* const);

	void updateCurrent(const sf::Time delta) override;
	void die();

private:
	bool mIsDead;
};

}
