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
	for (auto hintArea : hintAreasView)
	{
		for (auto player : playerView)
		{
			const auto& hintAreaBody = hintAreasView.get<component::BodyRect>(hintArea);
			const auto& playerBody = playerView.get<component::BodyRect>(player);
			auto& hint = hintAreasView.get<component::Hint>(hintArea);
			if (hintAreaBody.rect.contains(playerBody.rect.getCenter()))
			{
				PH_ASSERT_UNEXPECTED_SITUATION(mGui.hasInterface("hints"), "Player walked into hint area but gui does not have hints interface!");

				hint.isShown = true;
				mGui.showInterface("hints");
				auto* hintBackground = mGui.getInterface("hints")->getWidget("hintBackground");
				auto* hintContent = static_cast<TextWidget*>(hintBackground->getWidget("hintContent"));
				hintContent->setText(hint.content);
				

				//mGui.getInterface("hints")->getWidget("canvas")->getWidget(hintDetails.hintName.c_str())->show();

				//// NOTE: This is temporary
				//if(hintDetails.hintName == "controlHint") {
				//	ActionEventManager::setActionEnabled("changeWeapon", false);
				//	ActionEventManager::setActionEnabled("gunAttack", false);
				//	ActionEventManager::setActionEnabled("meleeAttack", false);
				//}
				//else if(hintDetails.hintName == "shootingHint") {
				//	ActionEventManager::setActionEnabled("gunAttack", true);
				//}
				//else if(hintDetails.hintName == "meleeFightingHint") {
				//	ActionEventManager::setActionEnabled("meleeAttack", true);
				//}
				//else if(hintDetails.hintName == "weaponChangingHint") {
				//	ActionEventManager::setActionEnabled("changeWeapon", true);
				//}
			}
			else if(hint.isShown)
			{
				//mGui.getInterface("hints")->getWidget("canvas")->getWidget(hintDetails.hintName.c_str())->hide();
				mGui.hideInterface("hints");
				hint.isShown = false;
			}
		}
	}
}

}

