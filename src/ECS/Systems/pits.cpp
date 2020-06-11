#include "pch.hpp"
#include "pits.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph::system {

using namespace component;

void Pits::update(float dt)
{
	PH_PROFILE_FUNCTION();

	mRegistry.view<PitChunk, BodyRect>().each([&]
	(const auto& pitChunk, auto pitChunkBody)
	{
		// NOTE: KinematicCollisionBody is here to make player not fall into pits when we're using pf (player flying) terminal command
		mRegistry.view<BodyRect, BodyCircle, KinematicCollisionBody> 
		(entt::exclude<IsOnPlatform, CurrentlyDashing, FallingIntoPit>).each([&]
		(auto objectEntity, auto objectRect, auto objectCircle, auto)
		{
			if(intersect(pitChunkBody, objectRect))
			{
				for(const auto& pitBody : pitChunk.pits)
				{
					if(intersect(pitBody, objectRect, objectCircle))
					{
						mRegistry.assign<FallingIntoPit>(objectEntity);
						if(auto* rq = mRegistry.try_get<RenderQuad>(objectEntity))
							rq->z = 188;
						return;
					}
				}
			}
		});
	});

	mRegistry.view<BodyRect, FallingIntoPit>().each([&]
	(auto entity, auto& body, auto& falling)
	{
		falling.timeToEnd -= dt;
		body.setSizeWithFixedCenter(Vec2(20.f) * falling.timeToEnd);
		if(falling.timeToEnd < 0.f)
		{
			mRegistry.assign_or_replace<DamageTag>(entity, 1000, false);
			mRegistry.remove<FallingIntoPit>(entity);
		}
	});
}

}
