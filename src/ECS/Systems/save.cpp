#include "pch.hpp"
#include "save.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Renderer/renderer.hpp"

namespace ph::system {

void Save::update(float dt)
{
	mRegistry.view<component::SavePoint, component::BodyRect>().each([&]
	(auto& save, const auto& saveBody)
	{
		mRegistry.view<component::Player, component::BodyRect>().each([&]
		(auto, const auto& playerBody)
		{
			if(save.timeSincePlayerSteppedOnIt < 2.f)
			{
				save.timeSincePlayerSteppedOnIt += dt;
				auto alpha = 255 - static_cast<unsigned char>(save.timeSincePlayerSteppedOnIt * 127.f);
				Renderer::submitText("GAME SAVED", "LiberationMono-Bold.ttf", {800.f, 10.f}, 70,
				                     sf::Color(50, 50, 255, alpha), 10, ProjectionType::gui);
			}

			if(intersect(saveBody, playerBody))
			{
				if(!save.isIntersectingPlayer)
				{
					save.isIntersectingPlayer = true;
					save.timeSincePlayerSteppedOnIt = 0.f;
				}
			}
			else
			{
				save.isIntersectingPlayer = false;	
			}
		});
	});
}

}
