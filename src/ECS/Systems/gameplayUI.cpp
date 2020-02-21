#include "gameplayUI.hpp"
#include "GUI/gui.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "Utilities/profiling.hpp"
#include <cstdio>

namespace ph::system {

void GameplayUI::update(float dt)
{
	PH_PROFILE_FUNCTION(1);

	if(sPause)
		return;

	auto view = mRegistry.view<component::Player, component::Bullets>();

	if (!GUI::hasInterface("gameplayCounters"))
		return;

	for(auto player : view)
	{
		auto* gameplayCounters = GUI::getInterface("gameplayCounters");
		char string[6];

		// set bullets counter
		const auto bullets = view.get<component::Bullets>(player);
		std::sprintf(string, "%i", bullets.numOfPistolBullets);
		dynamic_cast<TextWidget*>(gameplayCounters->getWidget("pistolBulletCounter"))->setText(string);
		std::sprintf(string, "%i", bullets.numOfShotgunBullets);
		dynamic_cast<TextWidget*>(gameplayCounters->getWidget("shotgunBulletCounter"))->setText(string);

		// set health counter
		if(mRegistry.has<component::Health>(player)) {
			const int playerHP = mRegistry.get<component::Health>(player).healthPoints;
			std::sprintf(string, "%i", playerHP);
			dynamic_cast<TextWidget*>(gameplayCounters->getWidget("vitalityCounter"))->setText(string);
		}
		else {
			std::sprintf(string, "%i", 0);
			dynamic_cast<TextWidget*>(gameplayCounters->getWidget("vitalityCounter"))->setText(string);
		}

		// NOTE: We have to take player's health from registry because health component is removed from player after death
	}
}

}

