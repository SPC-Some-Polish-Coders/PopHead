#include "pch.hpp"
#include "teleport.hpp"
#include "ECS/Components/debugComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

extern bool debugWindowOpen; 

namespace ph::system {

void Teleport::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(debugWindowOpen && ImGui::BeginTabItem("teleport"))
	{
		mRegistry.view<component::TeleportPoint, component::BodyRect>().each([&]
		(const auto& teleport, const auto& teleportBody)
		{
			if(ImGui::Button(teleport.name.c_str()))
			{
				mRegistry.view<component::Player, component::BodyRect>().each([&]
				(auto, auto& playerBody)
				{
					playerBody.pos = teleportBody.pos;
				});
			}
		});

		ImGui::EndTabItem();
	}
}

}
