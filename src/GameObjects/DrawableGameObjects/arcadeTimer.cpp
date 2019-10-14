#include "GameObjects/DrawableGameObjects/arcadeTimer.hpp"
#include "Gui/gui.hpp"
#include "Gui/interface.hpp"
#include <math.h>
#include <iostream>

namespace ph {

ArcadeTimer::ArcadeTimer(GUI& gui)
	:GameObject("arcadeTimer")
	,mGui(gui)
{
}

void ArcadeTimer::updateCurrent(const sf::Time delta)
{
	auto* interface = mGui.getInterface("arcadeCounters");
	auto* canvas = interface->getWidget("canvas");
	float elapsedTime = mArcadeClock.getElapsedTime().asSeconds();
	int elapsedTimeSeconds = static_cast<int>(elapsedTime);
	int elapsedTimeMiliseconds = static_cast<int>((elapsedTime - elapsedTimeSeconds) * 100);
	
	auto* arcadeClock = dynamic_cast<TextWidget*>(canvas->getWidget("arcadeClock"));
	if (elapsedTimeMiliseconds < 10 || elapsedTimeSeconds < 10)
		arcadeClock->setString("Time - 0" + std::to_string(elapsedTimeSeconds) + ":0" + std::to_string(elapsedTimeMiliseconds));
	if(elapsedTimeSeconds < 10)
		arcadeClock->setString("Time - 0" + std::to_string(elapsedTimeSeconds) + ":" + std::to_string(elapsedTimeMiliseconds));
	else if(elapsedTimeMiliseconds < 10)
		arcadeClock->setString("Time - " + std::to_string(elapsedTimeSeconds) + ":0" + std::to_string(elapsedTimeMiliseconds));
	else
		arcadeClock->setString("Time - " + std::to_string(elapsedTimeSeconds) + ":" + std::to_string(elapsedTimeMiliseconds));
}

}