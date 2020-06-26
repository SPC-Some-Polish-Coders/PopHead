#include "pch.hpp"
#include "savePoints.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Scenes/save.hpp"

namespace ph::system {

using namespace component;

void SavePoints::update(float dt)
{
	mRegistry.view<SavePoint, InsideSimRegion, BodyRect>().each([&]
	(auto& savePoint, auto, auto savePointBody)
	{
		mRegistry.view<Player, BodyRect, BodyCircle>().each([&]
		(auto, auto playerBody, auto playerCircle)
		{
			if(savePoint.timeSincePlayerSteppedOnIt < 2.f)
			{
				savePoint.timeSincePlayerSteppedOnIt += dt;
				u8 alpha = 255 - Cast<u8>(savePoint.timeSincePlayerSteppedOnIt * 127.f);
				Renderer::submitText("GAME SAVED", "LiberationMono-Bold.ttf", {800.f, 10.f}, 70,
				                     sf::Color(50, 50, 255, alpha), 10, ProjectionType::gui);
			}

			if(intersect(savePointBody, playerBody, playerCircle))
			{
				if(!savePoint.isIntersectingPlayer)
				{
					savePoint.isIntersectingPlayer = true;
					savePoint.timeSincePlayerSteppedOnIt = 0.f;
					saveGame(&mRegistry, savePointBody.pos);
				}
			}
			else
			{
				savePoint.isIntersectingPlayer = false;	
			}
		});
	});
}

}
