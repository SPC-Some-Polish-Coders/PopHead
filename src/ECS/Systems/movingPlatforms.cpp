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
		Vec2 toCenterOfPathBody = Math::distanceBetweenPoints(platformBody.center(), platform.pathBody.center());
		Vec2 velFactor = Vec2(1.f) - Math::hadamardDiv(toCenterOfPathBody, platform.pathBody.size);
		platform.currentVelocity = Math::hadamardMul(platform.fullVelocity, velFactor) * dt;
		platformBody.pos += platform.currentVelocity; 

		// flip platform velocity when platform is on the edge of path body
		if((platform.fullVelocity.x > 0.f && platformBody.right() > platform.pathBody.right()) ||
		   (platform.fullVelocity.x < 0.f && platformBody.x < platform.pathBody.x))
		{
			platform.fullVelocity.x = -platform.fullVelocity.x;
		}

		if((platform.fullVelocity.y > 0.f && platformBody.bottom() > platform.pathBody.bottom()) ||
		   (platform.fullVelocity.y < 0.f && platformBody.y < platform.pathBody.y))
		{
			platform.fullVelocity.y = -platform.fullVelocity.y;
		}
	}); 

	// platforms move bodies standing on them
	mRegistry.view<BodyRect, BodyCircle>().each([&]
	(auto entity, auto& body, auto circle)
	{
		circle.radius += sBodyCircleOnPlatformRadiusAddition;
		bool isOnPlatformBody = false;
		auto platforms = mRegistry.view<MovingPlatform, BodyRect>();
		for(auto platformEntity : platforms)
		{
			const auto& platform = platforms.get<MovingPlatform>(platformEntity);
			auto platformBody = platforms.get<BodyRect>(platformEntity); 
			if(intersect(platformBody, body, circle))
			{
				body.pos += platform.currentVelocity;
				isOnPlatformBody = true;
				break;
			}
		}

		if(isOnPlatformBody)
			mRegistry.assign_or_replace<IsOnPlatform>(entity);
		else
			mRegistry.reset<IsOnPlatform>(entity);
	});
}

}

