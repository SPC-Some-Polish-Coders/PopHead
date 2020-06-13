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
		ImGui::Columns(3);
		u32 toChangeColumn = 7;
		mRegistry.view<TeleportPoint, BodyRect>().each([&]
		(const auto& teleport, const auto& teleportBody)
		{
			--toChangeColumn;
			if(toChangeColumn == 0)
			{
				ImGui::NextColumn();
				toChangeColumn = 7;
			}

			if(ImGui::Button(teleport.name.c_str()))
			{
				auto& playerBody = mRegistry.get<BodyRect>(getPlayerEntity());
				playerBody.pos = teleportBody.pos;
			}
		});
		ImGui::Columns(1);

		ImGui::EndTabItem();
	}
}

}
