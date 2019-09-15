#include "GameObjects/DrawableGameObjects/Characters/Npcs/gateGuard.hpp"
#include "GameObjects/DrawableGameObjects/gate.hpp"
#include "Utilities/math.hpp"

namespace ph {

GateGuard::GateGuard(GameData* const gameData)
	:Npc(gameData, "gateGuard")
	,mOpened(false)
{
}

void GateGuard::updateCurrent(const sf::Time delta)
{
	auto* playerGameObject = mRoot->getChild("LAYER_standingObjects")->getChild("player");
	if (playerGameObject == nullptr)
		return;
	auto * player = dynamic_cast<Character*>(playerGameObject);
	auto* invisibleObjects = mRoot->getChild("LAYER_invisibleObjects");

	if (!mOpened)
	{
		auto* openGateArea = invisibleObjects->getChild("openGateArea");
		//if(Math::areTheyOverlapping(player->getGlobalBounds(), openGateArea->getPosition()))
		//	openGate();
	}

	else
	{
		auto* closeGateArea = invisibleObjects->getChild("closeGateArea");
		//if (Math::areTheyOverlapping(player->getGlobalBounds(), openGateArea->getPosition()))
		//	closeGate();
	}
}

void GateGuard::openGate()
{
	mOpened = true;
	auto* lyingObjects = mRoot->getChild("LAYER_lyingObjects");
	auto* gate = dynamic_cast<Gate*>(lyingObjects->getChild("gate"));
	gate->close();
}


void GateGuard::closeGate()
{
	auto* lyingObjects = mRoot->getChild("LAYER_lyingObjects");
	auto* gate = dynamic_cast<Gate*>(lyingObjects->getChild("gate"));
	gate->open();
}

}