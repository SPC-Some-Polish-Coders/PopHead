#include "hintAreas.hpp"
#include "GUI/gui.hpp"
#include "GUI/widget.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "Events/actionEventManager.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

HintAreas::HintAreas(entt::registry& registry, GUI& gui)
:System(registry)
,mGui(gui)
{
}

void HintAreas::update(float dt)
{
	PH_PROFILE_FUNCTION(0);

	auto playerView = mRegistry.view<component::Player, component::BodyRect>();
	auto hintAreasView = mRegistry.view<component::Hint, component::BodyRect>();
	for (auto player : playerView)
	{
		const auto& playerBody = playerView.get<component::BodyRect>(player);
		for (auto hintArea : hintAreasView)
		{
			const auto& hintAreaBody = hintAreasView.get<component::BodyRect>(hintArea);
			auto& hintDetails = hintAreasView.get<component::Hint>(hintArea);
			if (hintAreaBody.rect.contains(playerBody.rect.getCenter()))
			{
				if (hintDetails.isShown)
					break;

				mGui.getInterface("hints")->show();
				mGui.getInterface("hints")->getWidget("canvas")->getWidget(hintDetails.hintName.c_str())->show();
				hintDetails.isShown = true;

				// NOTE: This is temporary
				if(hintDetails.hintName == "controlHint") {
					ActionEventManager::setActionEnabled("changeWeapon", false);
					ActionEventManager::setActionEnabled("gunAttack", false);
					ActionEventManager::setActionEnabled("meleeAttack", false);
				}
				else if(hintDetails.hintName == "shootingHint") {
					ActionEventManager::setActionEnabled("gunAttack", true);
				}
				else if(hintDetails.hintName == "meleeFightingHint") {
					ActionEventManager::setActionEnabled("meleeAttack", true);
				}
				else if(hintDetails.hintName == "weaponChangingHint") {
					ActionEventManager::setActionEnabled("changeWeapon", true);
				}
			}
			else if (hintDetails.isShown)
			{
				mGui.getInterface("hints")->getWidget("canvas")->getWidget(hintDetails.hintName.c_str())->hide();
				mGui.getInterface("hints")->hide();
				hintDetails.isShown = false;
			}
		}
	}
}

}

