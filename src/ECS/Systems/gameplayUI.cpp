#include "gameplayUI.hpp"
#include "GUI/gui.hpp"
#include "GUI/textWidget.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

GameplayUI::GameplayUI(entt::registry& registry, GUI& gui)
	:System(registry)
	,mGui(gui)
{
}

void GameplayUI::update(float dt)
{
	PH_PROFILE_FUNCTION(0);

	auto view = mRegistry.view<component::Player, component::Bullets>();

	if (!mGui.hasInterface("gameplayCounters"))
		return;

	for(auto player : view)
	{
		auto* canvas = mGui.getInterface("gameplayCounters")->getWidget("canvas");

		// set bullets counter
		const auto bullets = view.get<component::Bullets>(player);
		dynamic_cast<TextWidget*>(canvas->getWidget("pistolBulletCounter"))->setString(std::to_string(bullets.numOfPistolBullets));
		dynamic_cast<TextWidget*>(canvas->getWidget("shotgunBulletCounter"))->setString(std::to_string(bullets.numOfShotgunBullets));

		// set health counter
		if(mRegistry.has<component::Health>(player)) {
			const auto playerHP = mRegistry.get<component::Health>(player).healthPoints;
			dynamic_cast<TextWidget*>(canvas->getWidget("vitalityCounter"))->setString(std::to_string(playerHP));
		}
		else
			dynamic_cast<TextWidget*>(canvas->getWidget("vitalityCounter"))->setString(std::to_string(0));

		// NOTE: We have to take player's health from registry because health component is removed from player after death
	}
}

}

