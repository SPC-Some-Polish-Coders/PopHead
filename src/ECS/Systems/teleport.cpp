#include "pch.hpp"
#include "teleport.hpp"
#include "ECS/Components/debugComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/entityUtil.hpp"

extern bool debugWindowOpen; 

namespace ph::system {

using namespace component;

void Teleport::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(debugWindowOpen && ImGui::BeginTabItem("teleport"))
	{
		mRegistry.view<TeleportPoint, BodyRect>().each([&]
		(const auto& teleport, const auto& teleportBody)
		{
			if(ImGui::Button(teleport.name.c_str()))
			{
				auto& playerBody = mRegistry.get<BodyRect>(getPlayerEntity());
				playerBody.pos = teleportBody.pos;
			}
		});

		ImGui::EndTabItem();
	}
}

}
