#include "pch.hpp"
#include "gameplayUI.hpp"
#include "GUI/gui.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "ECS/entityUtil.hpp"

namespace ph::system {

using namespace component;

void GameplayUI::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause || !GUI::hasInterface("gameplayCounters")) return;

	auto playerEntity = getPlayerEntity();
	auto playerBullets = mRegistry.get<Bullets>(playerEntity); 

	auto* gameplayCounters = GUI::getInterface("gameplayCounters");
	char str[6];

	// set bullets counter
	std::sprintf(str, "%i", playerBullets.numOfPistolBullets);
	dynamic_cast<TextWidget*>(gameplayCounters->getWidget("pistolBulletCounter"))->setText(str);
	std::sprintf(str, "%i", playerBullets.numOfShotgunBullets);
	dynamic_cast<TextWidget*>(gameplayCounters->getWidget("shotgunBulletCounter"))->setText(str);

	// set health counter
	if(const auto* playerHealth = mRegistry.try_get<Health>(playerEntity)) 
	{
		std::sprintf(str, "%i", playerHealth->healthPoints);
		dynamic_cast<TextWidget*>(gameplayCounters->getWidget("vitalityCounter"))->setText(str);
	}
	else 
	{
		std::sprintf(str, "%i", 0);
		dynamic_cast<TextWidget*>(gameplayCounters->getWidget("vitalityCounter"))->setText(str);
	}

	// NOTE: We have to take player's health through try_get because health component is removed from player after death
}

}

