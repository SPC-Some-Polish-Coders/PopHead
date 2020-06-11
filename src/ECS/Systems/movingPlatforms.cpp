#include "pch.hpp"
#include "movingPlatforms.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

using namespace component;

void MovingPlatforms::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<MovingPlatform, BodyRect>().each([&]
	(auto& platform, auto& platformBody)
	{
		if(!platform.active) return;

		// move platform
		platformBody.pos += platform.velocity * dt;

		// platform moves bodies standing on it
		mRegistry.view<BodyRect, BodyCircle>().each([&]
		(auto movedEntity, auto& body, auto& circle)
		{
			if(intersect(platformBody, body, circle))
			{
				body.pos += platform.velocity * dt;
				mRegistry.assign_or_replace<IsOnPlatform>(movedEntity);
			}
			else
			{
				mRegistry.reset<IsOnPlatform>(movedEntity);
			}
		});

		// flip platform velocity when platform is on the edge of path body
		if((platform.velocity.x > 0.f && platformBody.right() > platform.pathBody.right()) ||
		   (platform.velocity.x < 0.f && platformBody.x < platform.pathBody.x))
		{
			platform.velocity.x = -platform.velocity.x;
		}

		if((platform.velocity.y > 0.f && platformBody.bottom() > platform.pathBody.bottom()) ||
		   (platform.velocity.y < 0.f && platformBody.y < platform.pathBody.y))
		{
			platform.velocity.y = -platform.velocity.y;
		}
	}); 
}

}

