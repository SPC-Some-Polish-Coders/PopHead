#pragma once

#include "GameObjects/DrawableGameObjects/Characters/npc.hpp"

namespace ph {
	
class GameData;

class GateGuard : public Npc
{
public:
	GateGuard(GameData* const gameData);

	void updateCurrent(const sf::Time delta) override;

private:
	void openGate();
	void closeGate();

private:
	bool mOpened;

};

}