#include "GameObjects/DrawableGameObjects/arcadeTimer.hpp"
#include "Gui/gui.hpp"
#include "Gui/interface.hpp"
#include <math.h>

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
	int elapsedTimeMiliseconds = static_cast<int>((elapsedTime - elapsedTimeSeconds) * 10);
	
	auto* arcadeClock = dynamic_cast<TextWidget*>(canvas->getWidget("arcadeClock"));
	arcadeClock->setString("Time elapsed: " + std::to_string(elapsedTimeSeconds) + ":" + std::to_string(elapsedTimeMiliseconds));
}

}