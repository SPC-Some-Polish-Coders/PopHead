#include "pch.hpp"
#include "platforms.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

using namespace component;

void Platforms::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	// NOTE: KinematicCollisionBody is below to make player not fall into pits when you're using pf (player flying) terminal command

	// player - platform interactions
	mRegistry.view<BodyRect, BodyCircle, KinematicCollisionBody>().each([&]
	(auto entity, auto& body, auto circle, auto)
	{
		bool isOnPlatform = false;
		circle.radius += sBodyCircleRadiusAdditionForPlatforms;

		// move bodies that stand on moving platforms 
		// and register that they are on platform
		auto platforms = mRegistry.view<MovingPlatform, BodyRect>();
		for(auto platformEntity : platforms)
		{
			const auto& platform = platforms.get<MovingPlatform>(platformEntity);
			auto platformBody = platforms.get<BodyRect>(platformEntity); 
			if(intersect(platformBody, body, circle))
			{
				body.pos += platform.currentVelocity;
				isOnPlatform = true;
				break;
			}
		}

		// make falling platforms start falling apart when bodies stumble on them 
		// and register that bodies are on platform
		mRegistry.view<FallingPlatform, BodyRect, TextureRect>().each([&]
		(auto& platform, auto platformBody, auto platformTexRect)
		{
			if(platform.state == FallingPlatform::isStable || 
			   platform.state == FallingPlatform::isFallingApart)
			{
				if(intersect(platformBody, body, circle))
				{
					isOnPlatform = true;

					if(platform.state == FallingPlatform::isStable)
					{
						platform.state = FallingPlatform::isFallingApart;
						platform.timeToChangeState = 0.9f;
						platform.timeToChangeAnimationFrame = 0.3f;
					}
				}
			}
		});

		// make player be able or not be able to fall into pit 
		if(isOnPlatform)
			mRegistry.assign_or_replace<IsOnPlatform>(entity);
		else
			mRegistry.reset<IsOnPlatform>(entity);
	});

	// update moving platforms
	mRegistry.view<MovingPlatform, BodyRect>().each([&]
	(auto& platform, auto& platformBody)
	{
		if(!platform.active) return;

		// move platform
		Vec2 toCenterOfPathBody = distanceBetweenPoints(platformBody.center(), platform.pathBody.center());
		Vec2 velFactor = Vec2(1.f) - hadamardDiv(toCenterOfPathBody, platform.pathBody.size);
		platform.currentVelocity = hadamardMul(platform.fullVelocity, velFactor) * dt;
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

	// update falling platforms
	mRegistry.view<FallingPlatform, TextureRect>().each([&]
	(auto& platform, auto& texRect)
	{
		if(platform.state == FallingPlatform::isFallingApart ||
		   platform.state == FallingPlatform::isRecovering)
		{
			platform.timeToChangeState -= dt;
			if(platform.timeToChangeState < 0.f)
			{
				if(platform.state == FallingPlatform::isFallingApart)
				{
					platform.state = FallingPlatform::isRecovering;
					platform.timeToChangeState = 1.5f;
					texRect.x += 48;
				}
				else if(platform.state == FallingPlatform::isRecovering)
				{
					platform.state = FallingPlatform::isStable;
					texRect.x = 0;
				}
			}

			if(platform.state == FallingPlatform::isFallingApart)
			{
				platform.timeToChangeAnimationFrame -= dt;
				if(platform.timeToChangeAnimationFrame < 0.f)
				{
					texRect.x += 48;
					platform.timeToChangeAnimationFrame = 0.3f;
				}
			}
		}
	});
}

}
