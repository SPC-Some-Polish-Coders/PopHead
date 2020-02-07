#include "hintAreas.hpp"
#include "GUI/gui.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Systems/weather.hpp"
#include "Events/actionEventManager.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

void HintAreas::update(float dt)
{
	PH_PROFILE_FUNCTION(0);

	if(sPause)
		return;

	auto playerView = mRegistry.view<component::Player, component::BodyRect>();
	auto hintAreasView = mRegistry.view<component::Hint, component::BodyRect>();
	for (auto hintArea : hintAreasView)
	{
		for (auto player : playerView)
		{
			const auto& hintAreaBody = hintAreasView.get<component::BodyRect>(hintArea);
			const auto& playerBody = playerView.get<component::BodyRect>(player);
			auto& hint = hintAreasView.get<component::Hint>(hintArea);
			if (hintAreaBody.rect.contains(playerBody.rect.getCenter()))
			{
				PH_ASSERT_UNEXPECTED_SITUATION(GUI::hasInterface("hints"), "Player walked into hint area but gui does not have hints interface!");

				hint.isShown = true;
				auto* hints = GUI::getInterface("hints");
				hints->show();
				auto* hintBackground = hints->getWidget("hintBackground");
				auto* hintContent = static_cast<TextWidget*>(hintBackground->getWidget("hintContent"));
				hintContent->setText(hint.content);

				if(hint.hintName == "controlHint") {
					ActionEventManager::setActionEnabled("changeWeapon", false);
					ActionEventManager::setActionEnabled("gunAttack", false);
					ActionEventManager::setActionEnabled("meleeAttack", false);
					Weather::setRainType(Rain::Heavy);
					Weather::setMode(Weather::Rainy);	
				}
				else if(hint.hintName == "meleeFightingHint") {
					ActionEventManager::setActionEnabled("meleeAttack", true);
					Weather::setRainType(Rain::Normal);
				}
				else if(hint.hintName == "shootingHint") {
					ActionEventManager::setActionEnabled("gunAttack", true);
					Weather::setRainType(Rain::Drizzle);
				}
				else if(hint.hintName == "weaponChangingHint") {
					ActionEventManager::setActionEnabled("changeWeapon", true);
					Weather::setMode(Weather::Sunny);	
				}
			}
			else if(hint.isShown)
			{
				GUI::hideInterface("hints");
				hint.isShown = false;
			}
		}
	}
}

}

