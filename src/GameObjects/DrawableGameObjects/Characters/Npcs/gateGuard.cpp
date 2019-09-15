#include "GameObjects/DrawableGameObjects/Characters/Npcs/gateGuard.hpp"
#include "GameObjects/DrawableGameObjects/gate.hpp"
#include "GameObjects/NonDrawableGameObjects/activateArea.hpp"
#include "Utilities/math.hpp"

namespace ph {

GateGuard::GateGuard(GameData* const gameData)
	:Npc(gameData, "gateGuard")
	,mOpened(false)
{
}

void GateGuard::updateCurrent(const sf::Time delta)
{
	auto* invisibleObjects = mRoot->getChild("LAYER_invisibleObjects");

	if (!mOpened)
	{
		auto* openGateArea = dynamic_cast<ActivateArea*>(invisibleObjects->getChild("activateArea_openGate"));
		if (openGateArea == nullptr)
			return;
		if(openGateArea->getActivated())
			openGate();
	}

	else
	{
		auto* closeGateArea = dynamic_cast<ActivateArea*>(invisibleObjects->getChild("activateArea_closeGate"));
		if (closeGateArea == nullptr)
			return;
		if (closeGateArea->getActivated())
			closeGate();
	}
}

void GateGuard::openGate()
{
	mOpened = true;
	auto* lyingObjects = mRoot->getChild("LAYER_lyingObjects");
	auto* gate = dynamic_cast<Gate*>(lyingObjects->getChild("gate"));
	gate->open();
}


void GateGuard::closeGate()
{
	auto* lyingObjects = mRoot->getChild("LAYER_lyingObjects");
	auto* gate = dynamic_cast<Gate*>(lyingObjects->getChild("gate"));
	gate->close();
}

}