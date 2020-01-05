#include "hintAreas.hpp"
#include "GUI/gui.hpp"
#include "GUI/widget.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

HintAreas::HintAreas(entt::registry& registry, GUI& gui)
:System(registry)
,mGui(gui)
{
}

void HintAreas::update(float dt)
{
	PH_PROFILE_FUNCTION();

	auto playerView = mRegistry.view<component::Player, component::BodyRect>();
	auto hintAreasView = mRegistry.view<component::Hint, component::BodyRect>();
	for (auto player : playerView)
	{
		const auto& playerBody = playerView.get<component::BodyRect>(player);
		for (auto hintArea : hintAreasView)
		{
			auto& [hintAreaBody, hintDetails] = hintAreasView.get<component::BodyRect, component::Hint>(hintArea);
			if (hintAreaBody.rect.contains(playerBody.rect.getCenter()))
			{
				if (hintDetails.isShown)
					break;

				mGui.getInterface("hints")->show();
				mGui.getInterface("hints")->getWidget("canvas")->getWidget(hintDetails.hintName)->show();
				hintDetails.isShown = true;
			}
			else if (hintDetails.isShown)
			{
				mGui.getInterface("hints")->getWidget("canvas")->getWidget(hintDetails.hintName)->hide();
				mGui.getInterface("hints")->hide();
				hintDetails.isShown = false;
			}
		}
	}
}

}

